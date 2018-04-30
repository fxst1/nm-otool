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
