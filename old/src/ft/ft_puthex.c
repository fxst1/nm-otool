/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2014/07/05 09:53:57 by fjacquem          #+#    #+#             */
/*   Updated: 2018/03/12 16:40:40 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static void		ft_puthex_rec(unsigned int number, const int fd)
{
	char	c;
	char	*map;

	map = "0123456789abcdef";
	if (number)
	{
		c = (char)(number & 0xF);
		number = (number >> 4);
		ft_puthex_rec(number, fd);
		write(fd, map + c, 1);
	}
}

void			ft_puthex_fd(unsigned int number, const int fd)
{
	ft_putstr_fd("0x", fd);
	if (number)
		ft_puthex_rec(number, fd);
	else
		ft_putstr_fd("0", fd);
}
