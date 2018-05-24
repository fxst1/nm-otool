/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_ar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:13:09 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 14:27:43 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

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
		}
		value = (uintptr_t)start + o->offset + 0x3C + size_name;
		free(o);
		free(objs);
		objs = next;
	}
	return (err);
}

static int			read_objects(t_nm_otool *data, t_freader reader,
						uint8_t *buf, t_list **objs)
{
	size_t		i;

	i = 0;
	while (i < reader.symbol_size)
	{
		if (read_object(data, buf + reader.symtab_offset + i, objs) != 0)
			return (1);
		i += 0x8;
	}
	return (0);
}

int					ft_otool_ar(t_nm_otool *data, uint8_t *buf)
{
	uint32_t	i;
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
	reader.symbol_size = *(uint32_t*)(buf + 0x44 + size_name);
	reader.strtab_offset = reader.symtab_offset + reader.symbol_size;
	if (read_objects(data, reader, buf, &objs))
		return (1);
	write(STDOUT_FILENO, "Archive : ", 10);
	ft_putendl_fd(data->filename, STDOUT_FILENO);
	print_objects(data, objs, buf);
	return (0);
}
