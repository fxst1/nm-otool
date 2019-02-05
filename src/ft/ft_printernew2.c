/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printernew2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 21:24:34 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 21:24:47 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void		ft_printerflush(t_printerbuffered *pbuf)
{
	if (pbuf->usedsize > 0)
		write(pbuf->fd, pbuf->origin, pbuf->usedsize);
	ft_bzero(pbuf->origin, pbuf->allocsize);
	pbuf->usedsize = 0;
	pbuf->pointer = pbuf->origin;
}

void		ft_printerclose(t_printerbuffered *pbuf)
{
	free(pbuf->origin);
}

void		ft_printerclose_flush(t_printerbuffered *pbuf)
{
	ft_printerflush(pbuf);
	ft_printerclose(pbuf);
}
