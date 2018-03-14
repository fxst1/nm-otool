/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_corrupt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 11:24:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 12:17:30 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void			binary_can_read(t_binary *bin, size_t nbytes)
{
	if (bin->size <= bin->actual + nbytes)
	{
		ft_putstr_fd("Corrupt file !\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	bin->actual += nbytes;
}

void			binary_is_corrupt(t_binary *bin, void *addr, size_t nbytes)
{
	if ((uintptr_t)addr + nbytes - (uintptr_t)bin->buffer > bin->size)
	{
		ft_putstr_fd("Corrupt file !\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
}
