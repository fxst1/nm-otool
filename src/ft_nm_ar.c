#include "ft_nm_otool.h"

int			compare_ar_symbols(void *p1, void *p2)
{
	t_object	*a;
	t_object	*b;

	a = (t_object*)p1;
	b = (t_object*)p2;
	return (a->offset - b->offset);
}

uint32_t	get_size_name(uint8_t *buf)
{
	uint32_t ret;

	ret = 0;
	if (ft_strncmp("#1/", (char*)buf, 3) == 0)
		ret = (uint32_t)ft_atoi((char*)buf + 3);
	return (ret);
}

int			read_object(t_nm_otool *data, uint8_t *buf, t_list **root)
{
	t_object	ar_object;

	if (binary_is_corrupt(data, buf, sizeof(t_object)))
		return (corruption_error(data, "Symbol Table\n"));
	ar_object.symbol = *(uint32_t*)(buf);
	ar_object.offset = *(uint32_t*)(buf + 4);
	ft_lstadd_sort(root, ft_lstnew(&ar_object, sizeof(t_object)), &compare_ar_symbols);
	return (0);
}

int			print_objects(t_nm_otool *data, t_list *objs, uint8_t *start)
{
	t_list		*next;
	t_object	*o;
	int			ok;
	uint32_t	size_name;
	uint32_t	value;

	value = 0;
	ok = 1;
	next = NULL;
	while (objs)
	{
		next = objs->next;
		o = (t_object*)objs->content;
		size_name = get_size_name(start + o->offset);
		if (ok && value != o->offset)
		{
			ok |= ft_nm(data, start + o->offset + 0x3C + size_name);
			if (ok)
			{
				write(STDOUT_FILENO, "\n", 1);
				ft_putstr_fd(data->filename, STDOUT_FILENO);
				write(STDOUT_FILENO, "(", 1);
				ft_putstr_fd((char*)start + o->offset + 0x3C, STDOUT_FILENO);
				write(STDOUT_FILENO, "):\n", 3);
				ft_nm_print(data);
				ft_nm_clear(data);				
			}
		}
		value = o->offset;
		free(o);
		free(objs);
		objs = next;
	}
	return (ok);
}

int			ft_nm_ar(t_nm_otool *data, uint8_t *buf)
{
	uint32_t	i;
	uint32_t	symbol_size;
	uint32_t	size_name;
	t_freader	reader;
	t_list		*objs;

	i = 0;
	objs = NULL;
	if (binary_is_corrupt(data, buf + 0x4, 0x88))
		return (corruption_error(data, "Symtab Header\n"));
	size_name = get_size_name(buf + 0x8);
	ft_bzero(&reader, sizeof(t_freader));
	reader.symtab_offset = 0x44 + size_name + 0x4;
	if (binary_is_corrupt(data, buf + 0x44 + size_name, 4))
		return (corruption_error(data, "Symbol Table Size\n"));
	symbol_size = *(uint32_t*)(buf + 0x44 + size_name);
	reader.strtab_offset = reader.symtab_offset + symbol_size;
	while (i < symbol_size)
	{
		if (read_object(data, buf + reader.symtab_offset + i, &objs) != 0)
			return (1);
		i += 0x8;
	}
	print_objects(data, objs, buf);
	return (0);
}
