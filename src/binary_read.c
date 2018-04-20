/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_read.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:56:58 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/20 19:41:21 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static int			error(const int fd, const char *err, const char *filename)
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

uint32_t			may_swap32(int swap, uint32_t value)
{
	if (swap == 1)
		return (ft_swap_bytes(value));
	return (value);
}

int					binary_parse(t_binary *h, int force_swap)
{
	h->type_id = 0;
	h->magic = may_swap32(force_swap, *(uint32_t*)h->buffer);
	if (h->magic == MH_MAGIC_64 || h->magic == MH_CIGAM_64)
		mach_read_64(h, h->magic == MH_CIGAM_64);
	else if (h->magic == MH_MAGIC || h->magic == MH_CIGAM)
		mach_read_32(h, h->magic == MH_CIGAM);
	else if (h->magic == FAT_MAGIC || h->magic == FAT_CIGAM)
		fat_read_64(h);
	else if (!ft_strncmp((char*)h->buffer, ARMAG, SARMAG))
		ar_read_64(h);
	else
		ft_putstr_fd("Undefined MAGIC\n", STDERR_FILENO);
	return (0);
}

int					binary_read(const char *filename, t_binary *h)
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
	h->ptr = h->buffer;
	if (h->buffer == NULL || h->buffer == MAP_FAILED)
		return (error(fd, "allocate buffer", NULL));
	else if (read(fd, h->buffer, h->size) < 0)
		return (error(fd, "read", filename));
	ret = 0;
	if (binary_parse(h, 0) != 0)
		ret = -1;
	close(fd);
	return (ret);
}
