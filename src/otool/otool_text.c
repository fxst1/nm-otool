/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_text.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/14 16:56:43 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 20:10:58 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <otool.h>

static void		putline(t_otool *data, t_section_info sect, size_t i)
{
	size_t		j;

	j = 0;
	ft_putstr_fd("0x", 1);
	ft_putnbr_base_offset_fd(sect.offset + i, BASE_HEX, 16, 1);
	while (j < 16 && i + j < sect.size)
	{
		write(1, " ", 1);
		if (data->bin.buffer[sect.offset + i + j] <= 0xf)
			ft_putstr_fd("0", 1);
		ft_putnbr_base_fd(data->bin.buffer[sect.offset + i + j],
			BASE_HEX, 1);
		j++;
	}
}

static void		otool_text_darwin(t_otool *data)
{
	t_section_info	sect;
	size_t			i;

	get_segment_section(&data->bin, "__TEXT", "__text", &sect);
	if (sect.size > 0)
	{
		ft_putstr_fd("(__TEXT,__text) section\n", 1);
		i = 0;
		while (i < sect.size)
		{
			putline(data, sect, i);
			write(1, "\n", 1);
			i += 16;
		}
	}
}

void			otool_text(t_otool *data)
{
	if (data->bin.type_id == TYPE_ID_MACH64 ||
		data->bin.type_id == TYPE_ID_MACH32)
		otool_text_darwin(data);
}
