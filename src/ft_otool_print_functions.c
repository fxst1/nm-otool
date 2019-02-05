/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_print_functions.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 20:20:08 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 20:35:52 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void			must_flush_buffer(char *origin, char **pointer,
					size_t flushsize)
{
	uintptr_t	ptr;

	ptr = (uintptr_t)*pointer;
	if (((uintptr_t)ptr - (uintptr_t)(origin)) >= flushsize)
	{
		ft_putstr_fd(origin, STDOUT_FILENO);
		*pointer = origin;
		ft_bzero(origin, 1024);
	}
}

void			buffer_write(char **pointer, const void *buf,
						size_t buf_size)
{
	size_t				i;
	const unsigned char	*tmp;

	i = 0;
	tmp = (const unsigned char*)buf;
	while (i < buf_size)
	{
		*((*pointer)++) = (char)*(tmp++);
		i++;
	}
}

void			buffer_write_str(char **pointer, const char *buf)
{
	while (*buf)
		*((*pointer)++) = *(buf++);
}

void			buffer_prepare(char *buf, size_t buf_size, char **pointer)
{
	ft_bzero(buf, buf_size);
	*pointer = buf;
}
