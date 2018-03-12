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

static void	parse_binary(t_binary *h)
{
	(void)h;
}

int			load_binary(const char *filename, t_binary *h)
{
	int		fd;
	t_stat	st;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	if (fstat(fd, &st) == -1)
	{
		close(fd);
		return (-1);
	}
	h->size = st.st_size;
	h->buffer = (uint8_t*)mmap(NULL, st.st_size,
						PROT_READ | PROT_WRITE | PROT_EXEC,
						MAP_PRIVATE | MAP_ANONYMOUS,
						fd, 0);
	if (h->buffer == NULL)
	{
		close(fd);
		return (-1);
	}
	parse_binary(h);
	return (0);
}
