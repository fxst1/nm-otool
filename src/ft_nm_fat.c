/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_fat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:28:52 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/02 17:54:22 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		print_arch(t_nm_otool *data, uint32_t n_arch, uint32_t arch)
{
	if (n_arch == 1)
	{
		ft_putstr_fd(data->filename, STDOUT_FILENO);
		write(STDOUT_FILENO, ":\n", 2);
		return ;
	}
	write(STDOUT_FILENO, "\n", 1);
	ft_putstr_fd(data->filename, STDOUT_FILENO);
	write(STDOUT_FILENO, " (for architecture ", 19);
	if (arch == CPU_TYPE_POWERPC)
		write(STDOUT_FILENO, "ppc", 3);
	else if (arch == CPU_TYPE_I386)
		write(STDOUT_FILENO, "i386", 4);
	else if (arch == CPU_TYPE_X86_64)
		write(STDOUT_FILENO, "x86_64", 6);
	else
		write(STDOUT_FILENO, "?", 1);
	write(STDOUT_FILENO, "):\n", 3);
}

static int		explore_archs(t_nm_otool *data, uint32_t n_arch, uint8_t *buf,
				int swap)
{
	uint8_t		*tmp;
	uint32_t	i;
	uint32_t	offset;
	uint32_t	archs[1024];

	tmp = buf + 8;
	i = 0;
	while (i < n_arch && i < 1024)
	{
		if (binary_is_corrupt(data, tmp, 0x14))
			return (corruption_error(data, "Fat content\n"));
		archs[i] = swap ? ft_swap_bytes(*(uint32_t*)tmp) :
							*(uint32_t*)tmp;
		offset = swap ? ft_swap_bytes(*((uint32_t*)tmp + 2)) :
							*((uint32_t*)tmp + 2);
		if (offset == 0)
			return (corruption_error(data, "Fat offset overflow or zero\n"));
		print_arch(data, n_arch, archs[i]);
		ft_nm(data, buf + offset);
		tmp += 0x14;
		i++;
	}
	return (0);
}

int				ft_nm_fat(t_nm_otool *data, uint8_t *buf, int swap)
{
	uint8_t		*tmp;
	uint32_t	i;
	uint32_t	n_arch;
	uint32_t	offset;
	uint32_t	archs[1024];

	if (binary_is_corrupt(data, buf, 8))
		return (corruption_error(data, "Fat Header\n"));
	i = 0;
	n_arch = swap ? ft_swap_bytes(*((uint32_t*)buf + 1)) :
						*((uint32_t*)buf + 1);
	tmp = buf + 8;
	data->show_filename = 0;
	while (i < n_arch && i < 1024)
	{
		if (binary_is_corrupt(data, tmp, 20))
			return (corruption_error(data, "Fat content\n"));
		archs[i] = swap ? ft_swap_bytes(*(uint32_t*)tmp) : *(uint32_t*)tmp;
		offset = swap ? ft_swap_bytes(*((uint32_t*)tmp + 2)) :
							*((uint32_t*)tmp + 2);
		if (archs[i] == CPU_TYPE_X86_64)
			return (ft_nm(data, buf + offset));
		tmp += 0x14;
		i++;
	}
	data->show_filename = 1;
	return (explore_archs(data, n_arch, buf, swap));
}
