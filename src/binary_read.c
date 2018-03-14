/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:56:58 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 12:57:30 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static int 		error(const int fd, const char *err, const char *filename)
{
	write(2, "Cannot ", 7);
	ft_putstr_fd(err, 2);
	if (filename)
	{
		write(2, " ", 1);
		ft_putstr_fd(filename, 2);
	}
	write(2, "\n", 1);
	if (fd > 0)
		close(fd);
	return (-1);
}

static int		parse_binary(t_binary *h)
{
	h->magic = *(uint32_t*)h->buffer;
	if (h->magic == MH_MAGIC_64)
		mach_read_64(h);
	else if (h->magic == MH_MAGIC)
		mach_read_32(h);
	else if (h->magic == ELF_MAGIC_64)
		elf_read_64(h);
	else if (h->magic == ELF_MAGIC_32)
		elf_read_32(h);
	else
		ft_putstr_fd("Undefined MAGIC\n", 2);
	return (0);
}

int				binary_read(const char *filename, t_binary *h)
{
	int			fd;
	int			ret;
	t_stat		st;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (error(fd, "open", filename));
	if (fstat(fd, &st) == -1)
		return (error(fd, "stat", filename));
	h->size = st.st_size;
	h->symbols = NULL;
	h->buffer = (uint8_t*)malloc(st.st_size);
	if (h->buffer == NULL || h->buffer == MAP_FAILED)
		return (error(fd, "allocate buffer", NULL));
	else if (read(fd, h->buffer, h->size) < 0)
		return (error(fd, "read", filename));
	ret = 0;
	if (parse_binary(h) != 0)
		ret = -1;
	close(fd);
	return (ret);
}
