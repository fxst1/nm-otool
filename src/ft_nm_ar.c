/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_ar.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:16:14 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 18:00:58 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void			myoutput(t_nm_otool *data, uint8_t *start, t_object *o)
{
	write(STDOUT_FILENO, "\n", 1);
	ft_putstr_fd(data->filename, STDOUT_FILENO);
	write(STDOUT_FILENO, "(", 1);
	ft_putstr_fd((char*)start + o->offset + RANLIB_HEADER_SIZE, STDOUT_FILENO);
	write(STDOUT_FILENO, "):\n", 3);
	ft_nm_print(data);
	ft_nm_clear(data);
}

static int			print_object(t_nm_otool *data, uint8_t *start,
						uintptr_t *value, t_object *o)
{
	uint32_t	size_name;
	int			err;

	size_name = get_size_name(start + o->offset);
	if (*value != (uintptr_t)start + o->offset + RANLIB_HEADER_SIZE + size_name)
	{
		data->print = 0;
		err = ft_nm(data, start + o->offset + RANLIB_HEADER_SIZE + size_name);
		if (!err)
			myoutput(data, start, o);
		else
			return (1);
	}
	*value = (uintptr_t)start + o->offset + RANLIB_HEADER_SIZE + size_name;
	return (0);
}

static int			print_objects(t_nm_otool *data, t_list *objs,
						uint8_t *start)
{
	t_list		*next;
	t_object	*o;
	int			err;
	uintptr_t	value;

	value = 0;
	err = 0;
	next = NULL;
	while (objs)
	{
		next = objs->next;
		o = (t_object*)objs->content;
		if (!err)
			err |= print_object(data, start, &value, o);
		free(o);
		free(objs);
		objs = next;
	}
	return (err);
}

int					ft_nm_ar(t_nm_otool *data, uint8_t *buf)
{
	uint32_t	i;
	uint32_t	symbol_size;
	uint32_t	size_name;
	t_freader	reader;
	t_list		*objs;

	i = 0;
	objs = NULL;
	data->show_filename = 0;
	if (binary_is_corrupt(data, buf + 0x4, 0x88))
		return (corruption_error(data, "Symtab Header\n"));
	size_name = get_size_name(buf + ARCH_SIGN_LEN);
	ft_bzero(&reader, sizeof(t_freader));
	reader.symtab_offset = RANLIB_LONG_NAME_OFFSET + size_name + 0x4;
	if (binary_is_corrupt(data, buf + RANLIB_LONG_NAME_OFFSET + size_name, 4))
		return (corruption_error(data, "Symbol Table Size\n"));
	symbol_size = *(uint32_t*)(buf + RANLIB_LONG_NAME_OFFSET + size_name);
	reader.strtab_offset = reader.symtab_offset + symbol_size;
	if (append_unref_objects(data, buf + reader.strtab_offset, &objs))
		return (1);
	ft_lstsort(&objs, compare_ar_symbols);
	print_objects(data, objs, data->buffer);
	return (0);
}
