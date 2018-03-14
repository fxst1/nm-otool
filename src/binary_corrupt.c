/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_corrupt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 11:24:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 12:56:10 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void			binary_strtab_corrupt(t_binary *bin, char *addr)
{
	if ((uintptr_t)addr - (uintptr_t)bin->buffer > bin->size)
	{
		binary_delete(bin);
		ft_putstr_fd("Corrupt file detected !\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	while (*addr)
	{
		if ((uintptr_t)addr - (uintptr_t)bin->buffer > bin->size)
		{
			binary_delete(bin);
			ft_putstr_fd("Corrupt file detected !\n", STDERR_FILENO);
			exit(EXIT_FAILURE);
		}
		addr++;
	}
}

void			binary_is_corrupt(t_binary *bin, void *addr, size_t nbytes)
{
	if ((uintptr_t)addr + nbytes - (uintptr_t)bin->buffer > bin->size)
	{
		binary_delete(bin);
		ft_putstr_fd("Corrupt file detected !\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
