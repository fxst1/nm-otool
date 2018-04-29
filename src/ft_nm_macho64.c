#include "ft_nm_otool.h"

int				look_symtab_64(t_nm_otool *data, uint8_t *buf, t_freader *reader)
{
	if (binary_is_corrupt(data, buf + 8, 16))
		return (corruption_error(data, "LC_SYMTAB\n"));
	reader->symtab_offset = *(uint32_t*)(buf + 8);
	reader->n_symbols = *(uint32_t*)(buf + 12);
	reader->strtab_offset = *(uint32_t*)(buf + 16);
	reader->strtab_size = *(uint32_t*)(buf + 20);
	return (0);
}

int				load_symtab_64(t_nm_otool *data, t_freader *reader, uint8_t *buf)
{
	int			ret;
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	ret = 1;
	while (i < reader->n_load_commands)
	{
		if (binary_is_corrupt(data, buf, 8))
			return (corruption_error(data, "Load Command\n"));
		else
		{
			type = *((uint32_t*)buf);
			size = *((uint32_t*)buf + 1);
			if (size == 0)
				return (corruption_error(data, "Command size is nul\n"));
			else if (binary_is_corrupt(data, buf, size))
				return (corruption_error(data, "Section\n"));
			else if (type == LC_SYMTAB)
			{
				if (look_symtab_64(data, buf, reader) != 0)
					return (1);
				ret = 0;
			}
			buf += size;
		}
		i++;
	}
	if (ret)
		return (corruption_error(data, "No LC_SYMTAB\n"));
	return (0);
}

int			get_seg_sect_name_64(uint8_t *buf, size_t sect_index, t_symbol *s,
								size_t n_load_commands)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;
	uint32_t	n_section;
	uint8_t		*tmp;

	i = 0;
	buf += 0x20;
	while (i < n_load_commands)
	{
		type = *((uint32_t*)buf);
		size = *((uint32_t*)buf + 1);
		if (type == LC_SEGMENT_64)
		{
			n_section = *(uint32_t*)(buf + 0x40);
			tmp = buf + 0x48;
			while (n_section--)
			{
				sect_index--;
				if (sect_index == 0)
				{
					s->segname = (char*)tmp;
					s->sectname = (char*)tmp + 0x10;
					return (0);
				}
				tmp += 0x50;
			}
		}
		buf += size;
		i++;
	}
	return (1);
}

int				load_symbols_64(t_nm_otool *data, t_freader *reader, uint8_t *buf,
							uint8_t *start)
{
	size_t		i;
	size_t		sect_index;
	t_symbol	s;

	i = 0;
	while (i < reader->n_symbols)
	{
		ft_bzero(&s, sizeof(s));
		if (binary_is_corrupt(data, buf, 0x10))
		{
			ft_putstr_fd("Corrupt file: Symbol\n", STDERR_FILENO);
			return (1);
		}
		s.symbname = (char*)start + reader->strtab_offset + *(uint32_t*)buf;
		s.type = *(buf + 4);
		s.value = *(uint64_t*)(buf + 8);
		sect_index = *(buf + 5);
		if (sect_index > 0 &&
			get_seg_sect_name_64(start, sect_index, &s, reader->n_load_commands) != 0)
		{
			corruption_error(data, "section index not found\n");
			return (1);
		}
		ft_lstadd_sort(&data->symbols, ft_lstnew(&s, sizeof(s)), &compare_symbols);
		buf += 0x10;
		i++;
	}
	return (0);
}

int 			ft_nm_macho64(t_nm_otool *data, uint8_t *buf)
{
	t_freader	reader;

	ft_bzero(&reader, sizeof(reader));
	if (binary_is_corrupt(data, buf, 24))
		return (corruption_error(data, "mach-o64 header\n"));
	else
	{
		reader.n_load_commands = *(uint32_t*)(buf + 0x10);
		reader.size_load_commands = *(uint32_t*)(buf + 0x14);
		if (load_symtab_64(data, &reader, buf + 0x20) != 0)
			return (1);
		if (binary_is_corrupt(data, buf + reader.strtab_offset, reader.strtab_size))
			return (corruption_error(data, "String Table\n"));
		if (load_symbols_64(data, &reader, buf + reader.symtab_offset, buf) != 0)
			return (1);
	}
	return (0);
}
