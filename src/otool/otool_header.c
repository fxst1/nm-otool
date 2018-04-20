/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/14 17:01:04 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 20:10:45 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <otool.h>

static void	show_mach64_header(t_mach64 *data)
{
	ft_putstr_fd("Mach64 header\n      magic ", 1);
	ft_puthex_fd(data->header.magic, 1);
	ft_putstr_fd("\n    cputype ", 1);
	ft_putnbr_fd(data->header.cputype, 1);
	ft_putstr_fd("\n cpusubtype ", 1);
	ft_putnbr_fd(data->header.cpusubtype & 0xFFFFFF, 1);
	ft_putstr_fd("\n       caps ", 1);
	ft_puthex_fd((data->header.cpusubtype >> 24), 1);
	ft_putstr_fd("\n   filetype ", 1);
	ft_puthex_fd(data->header.filetype, 1);
	ft_putstr_fd("\n      ncmds ", 1);
	ft_putnbr_fd(data->header.ncmds, 1);
	ft_putstr_fd("\n sizeofcmds ", 1);
	ft_putnbr_fd(data->header.sizeofcmds, 1);
	ft_putstr_fd("\n      flags ", 1);
	ft_putnbr_fd(data->header.flags, 1);
	ft_putstr_fd("\n", 1);
}

static void	show_mach32_header(t_mach32 *data)
{
	ft_putstr_fd("Mach32 header\n     magic ", 1);
	ft_puthex_fd(data->header.magic, 1);
	ft_putstr_fd("\n   cputype ", 1);
	ft_putnbr_fd(data->header.cputype, 1);
	ft_putstr_fd("\n cpusubtype ", 1);
	ft_putnbr_fd(data->header.cpusubtype & 0xFFFFFF, 1);
	ft_putstr_fd("\n       caps ", 1);
	ft_puthex_fd((data->header.cpusubtype >> 24), 1);
	ft_putstr_fd("\n   filetype ", 1);
	ft_puthex_fd(data->header.filetype, 1);
	ft_putstr_fd("\n      ncmds ", 1);
	ft_putnbr_fd(data->header.ncmds, 1);
	ft_putstr_fd("\n sizeofcmds ", 1);
	ft_putnbr_fd(data->header.sizeofcmds, 1);
	ft_putstr_fd("\n      flags ", 1);
	ft_putnbr_fd(data->header.flags, 1);
	ft_putstr_fd("\n", 1);
}

void		otool_header(t_otool *data)
{
	if (data->bin.type_id == TYPE_ID_MACH32)
		show_mach32_header(&data->bin.content.mach32);
	else if (data->bin.type_id == TYPE_ID_MACH64)
		show_mach64_header(&data->bin.content.mach64);
	else
		ft_putstr_fd("undefined format\n", 1);
	(void)data;
}
