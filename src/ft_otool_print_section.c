/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_print_section.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:33:23 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/12 17:23:26 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		putline(t_nm_otool *data, t_freader *reader, uint8_t *buf,
					size_t i)
{
	size_t		j;

	j = 0;
	ft_putnbr_base_offset_fd(reader->text_addr + i, BASE_HEX, data->nbits,
		STDOUT_FILENO);
	write(STDOUT_FILENO, "\t", 1);
	while (j < 16 && i + j < reader->text_size)
	{
		if (buf[(reader->text_offset & MASK_64_BITS) + i + j] <= 0xf)
			ft_putstr_fd("0", STDOUT_FILENO);
		ft_putnbr_base_fd(buf[(reader->text_offset & MASK_64_BITS) + i + j],
			BASE_HEX, STDOUT_FILENO);
		write(STDOUT_FILENO, " ", 1);
		j++;
	}
	if (data->opts & SHOW_CHARACTERS)
		while (j < 16)
		{
			write(STDOUT_FILENO, "   ", 3);
			j++;
		}
}

static void		putline_ppc(t_nm_otool *data, t_freader *reader, uint8_t *buf,
					size_t i)
{
	size_t		j;

	j = 0;
	ft_putnbr_base_offset_fd(reader->text_addr + i, BASE_HEX, data->nbits,
		STDOUT_FILENO);
	write(STDOUT_FILENO, "\t", 1);
	while (j < 16 && i + j < reader->text_size)
	{
		if (buf[(reader->text_offset & MASK_64_BITS) + i + j] <= 0xf)
			ft_putstr_fd("0", STDOUT_FILENO);
		ft_putnbr_base_fd(buf[(reader->text_offset & MASK_64_BITS) + i + j],
			BASE_HEX, STDOUT_FILENO);
		if (j % 4 == 3)
			write(STDOUT_FILENO, " ", 1);
		j++;
	}
	if (data->opts & SHOW_CHARACTERS)
		while (j < 16)
		{
			write(STDOUT_FILENO, "   ", 3);
			j++;
		}
}

static void		putchars(t_nm_otool *data, t_freader *reader, uint8_t *buf,
					size_t i)
{
	size_t		j;

	j = 0;
	while (j < 16 && i + j < reader->text_size)
	{
		if (ft_isprint(buf[reader->text_offset + i + j]))
			write(STDOUT_FILENO, &buf[reader->text_offset + i + j], 1);
		else
			write(STDOUT_FILENO, ".", 1);
		j++;
	}
	(void)data;
}

static void		print_header(t_nm_otool *data)
{
	if (data->print)
	{
		ft_putstr_fd(data->filename, STDOUT_FILENO);
		if (data->objname)
		{
			write(STDOUT_FILENO, "(", 1);
			ft_putstr_fd(data->objname, STDOUT_FILENO);
			write(STDOUT_FILENO, ")", 1);
		}
		write(STDOUT_FILENO, ":\n", 2);
	}
	write(STDOUT_FILENO, "Contents of (", 13);
	ft_putstr_fd(data->segment, STDOUT_FILENO);
	write(STDOUT_FILENO, ",", 1);
	ft_putstr_fd(data->section, STDOUT_FILENO);
	ft_putstr_fd(") section\n", STDOUT_FILENO);
}

int				ft_otool_print_section(t_nm_otool *data, t_freader *reader,
					uint8_t *start, int is_ppc)
{
	size_t	i;

	i = 0;
	print_header(data);
	while (i < reader->text_size)
	{
		if (binary_is_corrupt(data, start + i +
				(reader->text_offset & MASK_64_BITS), 16))
			return (corruption_error(data, "Section, Segment\n"));
		if (is_ppc == 0)
			putline(data, reader, start, i);
		else
			putline_ppc(data, reader, start, i);
		if (data->opts & SHOW_CHARACTERS)
		{
			write(STDOUT_FILENO, " ", 1);
			putchars(data, reader, start, i);
		}
		write(STDOUT_FILENO, "\n", 1);
		i += 16;
	}
	return (0);
}
