/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_ar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:13:09 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 18:17:04 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void			clear_object(t_list *objs)
{
	free(objs->content);
	free(objs);
}

static int			print_objects(t_nm_otool *data, t_list *objs,
						uint8_t *start)
{
	t_list		*next;
	t_object	*o;
	int			err;
	uint32_t	size_name;
	uintptr_t	value;

	value = 0;
	err = 0;
	while (objs)
	{
		next = objs->next;
		o = (t_object*)objs->content;
		size_name = get_size_name(start + o->offset);
		if (!err && value != (uintptr_t)start + o->offset + 0x3C + size_name)
		{
			data->objname = (char*)start + o->offset + 0x3C;
			if ((err = ft_otool(data, start + o->offset + 0x3C + size_name)))
				return (1);
			data->objname = NULL;
		}
		value = (uintptr_t)start + o->offset + 0x3C + size_name;
		clear_object(objs);
		objs = next;
	}
	return (err);
}

int					ft_otool_ar(t_nm_otool *data, uint8_t *buf)
{
	uint32_t	i;
	uint32_t	size_name;
	t_freader	reader;
	t_list		*objs;

	data->print = 1;
	i = 0;
	objs = NULL;
	if (binary_is_corrupt(data, buf + 0x4, 0x88))
		return (corruption_error(data, "Symtab Header\n"));
	size_name = get_size_name(buf + 0x8);
	ft_bzero(&reader, sizeof(t_freader));
	reader.symtab_offset = RANLIB_LONG_NAME_OFFSET + size_name + 0x4;
	if (binary_is_corrupt(data, buf + RANLIB_LONG_NAME_OFFSET + size_name, 4))
		return (corruption_error(data, "Symbol Table Size\n"));
	reader.symbol_size = *(uint32_t*)(buf + RANLIB_LONG_NAME_OFFSET +
							size_name);
	reader.strtab_offset = reader.symtab_offset + reader.symbol_size;
	if (append_unref_objects(data, buf + reader.strtab_offset, &objs))
		return (1);
	write(STDOUT_FILENO, "Archive : ", 10);
	ft_putendl_fd(data->filename, STDOUT_FILENO);
	print_objects(data, ft_lstsort(&objs, compare_ar_symbols), data->buffer);
	return (0);
}
