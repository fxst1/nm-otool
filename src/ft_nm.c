#include "ft_nm_otool.h"

void			ft_nm_print(t_nm_otool *data)
{
	if (data->opts == 0)
		ft_lstiter2(data->symbols, &iter_symbols_default, (void*)(uintptr_t)data->nbits);
}

void			ft_nm_clear(t_nm_otool *data)
{
	t_list		*next;
	t_list		*tmp;

	next = NULL;
	tmp = data->symbols;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
	data->symbols = NULL;
}

static int 		parse_magic(t_nm_otool *data, uint8_t *buf, uint32_t magic)
{
	if (magic == MH_MAGIC_64)
	{
		data->nbits = 16;
		return (ft_nm_macho64(data, buf));
	}
	else if (magic == MH_MAGIC)
	{
		data->nbits = 8;
		return (ft_nm_macho32(data, buf));
	}
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		return (ft_nm_fat(data, buf, magic == FAT_CIGAM));
	else if (ft_strncmp("!<arch>\n", (char*)buf, 8) == 0)
	{
		data->print = 0;
		return (ft_nm_ar(data, buf));
	}
	else
		display_error(data, -1, "Undefined MAGIC\n");
	return (1);
}

int 			ft_nm(t_nm_otool *data, uint8_t *buf)
{
	if (binary_is_corrupt(data, buf, 8))
		corruption_error(data, "Corrupt file: MAGIC\n");
	else
		parse_magic(data, buf, *(uint32_t*)buf);
	if (data->print)
	{
		ft_nm_print(data);
		ft_nm_clear(data);
		data->symbols = NULL;
	}
	return (0);
}
