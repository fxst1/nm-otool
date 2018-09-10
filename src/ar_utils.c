/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:15:24 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/10 22:45:03 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int			object_exist(t_list *root, t_object ar)
{
	t_object	*tmp;
	t_list		*lst;

	lst = root;
	while (lst)
	{
		tmp = (t_object*)lst->content;
		if (ar.offset == tmp->offset)
			return (1);
		lst = lst->next;
	}
	return (0);
}

int			read_object(t_nm_otool *data, uint8_t *buf, t_list **root)
{
	t_object	ar_object;

	if (binary_is_corrupt(data, buf, sizeof(t_object)))
		return (corruption_error(data, "Symbol Table\n"));
	ar_object.symbol = *(uint32_t*)(buf);
	ar_object.offset = *(uint32_t*)(buf + 4);
	if (!object_exist(*root, ar_object))
		ft_lstadd_sort(root, ft_lstnew(&ar_object, sizeof(t_object)),
						&compare_ar_symbols);
	return (0);
}

int			append_unref_objects(t_nm_otool *data, uint8_t *buf, t_list **root)
{
	t_object	o;
	uint32_t	strtab_size;
	uint32_t	object_size;
	uint32_t	size_name;

	strtab_size = *(uint32_t*)buf;
	buf += strtab_size + sizeof(uint32_t);
	while (!binary_is_corrupt(data, buf, RANLIB_HEADER_SIZE))
	{
		if (ft_strncmp("#1/", (char*)buf, 3))
			return (0);
		size_name = get_size_name(buf);
		buf += RANLIB_HEADER_SIZE_OFFSET;
		object_size = ft_atoi((char*)buf);
		buf += 0xC;
		o.symbol = 0;
		o.offset = (uint32_t)((buf - RANLIB_HEADER_SIZE) - data->buffer);
		ft_lstadd_sort(root, ft_lstnew(&o, sizeof(t_object)),
						&compare_ar_symbols);
		buf += object_size;
	}
	return (0);
}
