/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_offset_fd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/12/01 20:51:22 by fjacquem          #+#    #+#             */
/*   Updated: 2018/03/14 10:55:07 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>

static size_t 	getoffset(int nb, char *base, size_t *n)
{
	int length;

	length = ft_strlen(base);
	if (length > 0)
	{
		if (nb < 0)
			nb = -nb;
		if (nb >= length)
		{
			getoffset(nb / length, base, n);
			getoffset(nb % length, base, n);
		}
		else
			(*n)++;
	}
	return (*n);
}

void			ft_putnbr_base_offset_fd(int nb, char *base, size_t offset, int fd)
{
	size_t		tmp;

	tmp = 0;
	offset = offset - getoffset(nb, base, &tmp);
	while (offset--)
		write(fd, "0", 1);
	ft_putnbr_base_fd(nb, base, fd);
}
