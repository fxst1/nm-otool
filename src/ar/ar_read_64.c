/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar_read_64.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 13:35:25 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 18:43:22 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void		read_objects(t_binary *bin, t_ar *ar)
{
	uint32_t	i;
	uint32_t	offset;
	size_t		size_name;

	i = 0;
	offset = 4;
	while (i < ar->n_content)
	{
		ar->contents[i].info = *(t_ranlib*)(bin->buffer +
								ar->start_content + offset);
		binary_is_corrupt(bin, bin->buffer + ar->contents[i].info.offset,
							sizeof(t_ar_header));
		ar->contents[i].name = ar->strtab + ar->contents[i].info.strx + 4;
		ar->contents[i].header = *((t_ar_header*)(bin->buffer +
									ar->contents[i].info.offset));
		size_name = (int)atoi((char*)ar->contents[i].header.name + 3);
		ar->contents[i].bin = (t_binary*)malloc(sizeof(t_binary));
		ft_memcpy(ar->contents[i].bin, bin, sizeof(t_binary));
		ar->contents[i].bin->buffer = bin->buffer +
									ar->contents[i].info.offset +
									size_name +
									sizeof(t_ar_header);
		binary_parse(ar->contents[i].bin);
		offset += sizeof(t_ranlib);
		i++;
	}
}

static void		read_symbols(t_binary *bin, t_ar *ar)
{
	uint32_t	i;
	uint32_t	offset;
	uint32_t	size_symbols;

	i = 0;
	offset = 4;
	size_symbols = *(uint32_t*)(bin->buffer + ar->start_content);
	ar->n_content = size_symbols / sizeof(t_ranlib);
	ar->contents = (t_ar_object*)malloc(sizeof(t_ar_object) * ar->n_content);
	while (offset < size_symbols)
	{
		binary_is_corrupt(bin, bin->buffer + ar->start_content + offset,
							sizeof(t_ranlib));
		offset += sizeof(t_ranlib);
		i++;
	}
	binary_strtab_corrupt(bin, (char*)bin->buffer + ar->start_content + offset);
	ar->strtab = (char*)bin->buffer + ar->start_content + offset;
}

static void		read_header(t_binary *bin, t_ar *ar)
{
	size_t		size_name;

	ar->header = *((t_ar_header*)(bin->buffer + 8));
	size_name = (size_t)atoi((char*)bin->buffer + 11);
	ar->start_content = 8 + sizeof(t_ar_header) + size_name;
	binary_is_corrupt(bin, bin->buffer + ar->start_content, 4);
}

int				ar_read_64(t_binary *bin)
{
	bin->type_id = TYPE_ID_RANLIB;
	binary_is_corrupt(bin, bin->buffer + 8, sizeof(t_ar_header));
	read_header(bin, &bin->content.ar);
	read_symbols(bin, &bin->content.ar);
	read_objects(bin, &bin->content.ar);
	return (0);
}
