#include "ft_nm_otool.h"

static int 		parse_magic(t_nm_otool *data, uint8_t *buf, uint32_t magic)
{
	if (magic == MH_MAGIC_64)
		return (ft_nm_macho64(data, buf));
	else if (magic == MH_MAGIC)
		return (ft_nm_macho32(data, buf));
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		return (ft_nm_fat(data, buf, magic == FAT_CIGAM));
	else if (ft_strncmp("!<arch>\n", (char*)buf, 8) == 0)
		return (ft_nm_ar(data, buf));
	else
		display_error(data, -1, "Undefined MAGIC\n");
	return (1);
}

int 			ft_nm(t_nm_otool *data, uint8_t *buf)
{
	t_list		*next;
	t_list		*tmp;

	next = NULL;
	if (binary_is_corrupt(data, buf, 8))
		corruption_error(data, "Corrupt file: MAGIC\n");
	else
		parse_magic(data, buf, *(uint32_t*)buf);
	tmp = data->symbols;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
	data->symbols = NULL;
	return (0);
}
