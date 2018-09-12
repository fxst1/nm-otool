/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_ppc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:30:16 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/12 17:18:59 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int			get_segment_ppc(t_nm_otool *data, uint8_t *buf, t_freader *reader)
{
	uint32_t	n_section;
	uint8_t		*tmp;

	n_section = ft_swap_bytes(*(uint32_t*)(buf + 0x30));
	tmp = buf + 0x38;
	while (n_section--)
	{
		if (binary_is_corrupt(data, tmp, 0x44))
			return (corruption_error(data, "Section\n"));
		if (ft_strcmp(data->section, (char*)tmp) == 0 &&
			ft_strcmp(data->segment, (char*)tmp + 0x10) == 0)
		{
			reader->text_addr = ft_swap_bytes(*(uint64_t*)(tmp + 0x20));
			reader->text_size = ft_swap_bytes(*(uint32_t*)(tmp + 0x24));
			reader->text_offset = ft_swap_bytes(*(uint32_t*)(tmp + 0x28));
			return (0);
		}
		tmp += 0x44;
	}
	return (-1);
}

int			get_section_ppc(t_nm_otool *data, uint8_t *buf, t_freader *reader)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;
	int			ret;

	ret = 1;
	i = 0;
	while (i < reader->n_load_commands)
	{
		if (binary_is_corrupt(data, buf, 8))
			return (corruption_error(data, "Load Command\n"));
		type = ft_swap_bytes(*((uint32_t*)buf));
		size = ft_swap_bytes(*((uint32_t*)buf + 1));
		if (binary_is_corrupt(data, buf, size))
			return (corruption_error(data, "Segment\n"));
		else if (type == LC_SEGMENT)
		{
			ret = get_segment_ppc(data, buf, reader);
			if (ret == 1 || ret == 0)
				return (ret);
		}
		buf += size ? size : reader->size_load_commands;
		i++;
	}
	return (ret);
}

int			ft_otool_ppc(t_nm_otool *data, uint8_t *buf)
{
	t_freader	reader;

	ft_bzero(&reader, sizeof(reader));
	if (binary_is_corrupt(data, buf, 24))
		return (corruption_error(data, "mach-o64 header\n"));
	else
	{
		reader.n_load_commands = ft_swap_bytes(*(uint32_t*)(buf + 0x10));
		reader.size_load_commands = ft_swap_bytes(*(uint32_t*)(buf + 0x14));
		if (get_section_ppc(data, buf + 0x1C, &reader) != 0)
			return (1);
		ft_otool_print_section(data, &reader, buf, 1);
	}
	return (0);
}
