/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_base_offset_bufferred.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 19:01:47 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 19:21:18 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		ft_putnbr_base_buffer2(int64_t nb, char *base, size_t length,
				char **buffer)
{
	if (length > 0)
	{
		if (nb >= (int64_t)length)
		{
			ft_putnbr_base_buffer2(nb / length, base, length, buffer);
			ft_putnbr_base_buffer2(nb % length, base, length, buffer);
		}
		else
			*((*buffer)++) = base[nb];
	}
}

void			ft_putnbr_base_buffer(int64_t nb, char *base, char **buffer)
{
	ft_putnbr_base_buffer2(nb, base, ft_strlen(base), buffer);
}

static size_t	getoffset(int64_t nb, char *base, size_t length, size_t *n)
{
	if (length > 0)
	{
		if (nb >= (int64_t)length)
		{
			getoffset(nb / length, base, length, n);
			getoffset(nb % length, base, length, n);
		}
		else
			(*n)++;
	}
	return (*n);
}

void			ft_putnbr_base_offset_buffer(int64_t nb, char *base,
					size_t offset, char **buffer)
{
	size_t		tmp;
	size_t		length;

	length = ft_strlen(base);
	tmp = 0;
	getoffset(nb, base, length, &tmp);
	while (tmp < offset)
	{
		*((*buffer)++) = '0';
		tmp++;
	}
	ft_putnbr_base_buffer2(nb, base, length, buffer);
}
