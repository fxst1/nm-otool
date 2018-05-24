/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_seg_sect_name.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:58:04 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 17:57:55 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int		get_names_32(t_symbol *s, uint8_t *buf, size_t *sect_index)
{
	uint32_t	n_section;
	uint8_t		*tmp;

	n_section = *(uint32_t*)(buf + 0x30);
	tmp = buf + 0x38;
	while (n_section--)
	{
		(*sect_index)--;
		if (*sect_index == 0)
		{
			s->segname = (char*)tmp;
			s->sectname = (char*)tmp + 0x10;
			return (0);
		}
		tmp += 0x44;
	}
	return (1);
}

int				get_seg_sect_name_32(uint8_t *buf, size_t sect_index,
					t_symbol *s, size_t n_load_commands)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	buf += 0x1C;
	while (i < n_load_commands)
	{
		type = *((uint32_t*)buf);
		size = *((uint32_t*)buf + 1);
		if (type == LC_SEGMENT)
		{
			if (get_names_32(s, buf, &sect_index) == 0)
				return (0);
		}
		buf += size;
		i++;
	}
	return (1);
}

static int		get_names_ppc(t_symbol *s, uint8_t *buf, size_t *sect_index)
{
	uint32_t	n_section;
	uint8_t		*tmp;

	printf("here\n");
	n_section = ft_swap_bytes(*(uint32_t*)(buf + 0x30));
	tmp = buf + 0x38;
	while (n_section--)
	{
		(*sect_index)--;
		if (*sect_index == 0)
		{
			s->segname = (char*)tmp;
			s->sectname = (char*)tmp + 0x10;
			return (0);
		}
		tmp += 0x44;
	}
	return (1);
}

int				get_seg_sect_name_ppc(uint8_t *buf, size_t sect_index,
					t_symbol *s, size_t n_load_commands)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	buf += 0x1C;
	printf("here\n");
	while (i < n_load_commands)
	{
		type = *((uint32_t*)buf);
		size = *((uint32_t*)buf + 1);
		if (type == LC_SEGMENT)
		{
			if (get_names_ppc(s, buf, &sect_index) == 0)
				return (0);
		}
		buf += size;
		i++;
	}
	return (1);
}

static int		get_names_64(t_symbol *s, uint8_t *buf, size_t *sect_index)
{
	uint32_t	n_section;
	uint8_t		*tmp;

	n_section = *(uint32_t*)(buf + 0x40);
	tmp = buf + 0x48;
	while (n_section--)
	{
		(*sect_index)--;
		if (*sect_index == 0)
		{
			s->segname = (char*)tmp;
			s->sectname = (char*)tmp + 0x10;
			return (0);
		}
		tmp += 0x50;
	}
	return (1);
}

int				get_seg_sect_name_64(uint8_t *buf, size_t sect_index,
					t_symbol *s, size_t n_load_commands)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	buf += 0x20;
	while (i < n_load_commands)
	{
		type = *((uint32_t*)buf);
		size = *((uint32_t*)buf + 1);
		if (type == LC_SEGMENT_64)
		{
			if (get_names_64(s, buf, &sect_index) == 0)
				return (0);
		}
		buf += size;
		i++;
	}
	return (1);
}
