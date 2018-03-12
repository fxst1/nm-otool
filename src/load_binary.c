/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_binary.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:56:58 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 11:40:17 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void		parse_binary(t_binary *h)
{
	h->magic = *(uint32_t*)h->buffer;
	if (h->magic == MH_MAGIC_64)
		mach_read_64(h);
	else if (h->magic == MH_MAGIC)
		mach_read_32(h);
	(void)h;
}

int				binary_read(const char *filename, t_binary *h)
{
	int			fd;
	t_stat		st;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Cannot open ", 12);
		ft_putstr_fd(filename, STDERR_FILENO);
		write(2, "\n", 1);
		return (-1);
	}
	if (fstat(fd, &st) == -1)
	{
		write(2, "Cannot stat ", 12);
		ft_putstr_fd(filename, STDERR_FILENO);
		write(2, "\n", 1);
		close(fd);
		return (-1);
	}
	h->size = st.st_size;
	h->buffer = (uint8_t*)malloc(st.st_size);
	if (h->buffer == NULL)
	{
		close(fd);
		return (-1);
	}
	else if (read(fd, h->buffer, h->size) < 0)
	{
		close(fd);
		return (-1);
	}
	parse_binary(h);
	close(fd);
	return (0);
}
