/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:30:59 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/12 16:51:11 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static int		parse_magic(t_nm_otool *data, uint8_t *buf, uint32_t magic)
{
	if (magic == MH_MAGIC_64)
	{
		data->nbits = 16;
		return (ft_otool_macho64(data, buf));
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		data->nbits = 8;
		return (magic == MH_CIGAM ? ft_otool_ppc(data, buf) :
									ft_otool_macho32(data, buf));
	}
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		return (ft_otool_fat(data, buf, magic == FAT_CIGAM));
	else if (ft_strncmp(ARCH_SIGN, (char*)buf, ARCH_SIGN_LEN) == 0)
	{
		data->print = 0;
		return (ft_otool_ar(data, buf));
	}
	else
		display_error(data, -1, "Undefined magic number\n");
	return (1);
}

int				ft_otool(t_nm_otool *data, uint8_t *buf)
{
	if (binary_is_corrupt(data, buf, 8))
		corruption_error(data, "magic\n");
	else
		parse_magic(data, buf, *(uint32_t*)buf);
	return (0);
}
