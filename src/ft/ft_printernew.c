/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printernew.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 20:40:15 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 21:25:20 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void		ft_printerinit(t_printerbuffered *pbuf, int fd, size_t allocsize,
				size_t flushsize)
{
	pbuf->fd = fd;
	pbuf->allocsize = allocsize;
	pbuf->flushsize = flushsize == 0 ? allocsize : flushsize;
	pbuf->usedsize = 0;
	pbuf->origin = ft_memalloc(allocsize + 1);
	pbuf->pointer = pbuf->origin;
}

void		ft_printerwrite(t_printerbuffered *pbuf, const void *buf,
				size_t buf_size)
{
	size_t				i;
	const unsigned char	*tmp;

	i = 0;
	tmp = (const unsigned char*)buf;
	while (i < buf_size)
	{
		if (pbuf->usedsize >= pbuf->flushsize)
			ft_printerflush(pbuf);
		*(pbuf->pointer++) = (char)*(tmp++);
		pbuf->usedsize++;
		i++;
	}
}

void		ft_printerwrite_str(t_printerbuffered *pbuf, const char *buf)
{
	while (*buf)
	{
		if (pbuf->usedsize >= pbuf->flushsize)
			ft_printerflush(pbuf);
		*(pbuf->pointer++) = (char)*(buf++);
		pbuf->usedsize++;
	}
}
