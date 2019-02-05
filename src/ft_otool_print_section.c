/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_print_section.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:33:23 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 21:14:04 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		putline(t_nm_otool *data, t_freader *reader, uint8_t *buf,
					size_t i)
{
	size_t		j;

	j = 0;
	ft_putnbr_base_offset_buffer(reader->text_addr + i, BASE_HEX, data->nbits,
		&data->pbuf);
	ft_printerwrite(&data->pbuf, "\t", 1);
	while (j < 16 && i + j < reader->text_size)
	{
		if (buf[(reader->text_offset & MASK_64_BITS) + i + j] <= 0xf)
			ft_printerwrite(&data->pbuf, "0", 1);
		ft_putnbr_base_buffer(buf[(reader->text_offset & MASK_64_BITS) + i + j],
			BASE_HEX, &data->pbuf);
		ft_printerwrite(&data->pbuf, " ", 1);
		j++;
	}
	if (data->opts & SHOW_CHARACTERS)
		while (j < 16)
		{
			ft_printerwrite_str(&data->pbuf, "   ");
			j++;
		}
}

static void		putline_ppc(t_nm_otool *data, t_freader *reader, uint8_t *buf,
					size_t i)
{
	size_t		j;

	j = 0;
	ft_putnbr_base_offset_buffer(reader->text_addr + i, BASE_HEX, data->nbits,
		&data->pbuf);
	ft_printerwrite(&data->pbuf, "\t", 1);
	while (j < 16 && i + j < reader->text_size)
	{
		if (buf[(reader->text_offset & MASK_64_BITS) + i + j] <= 0xf)
			ft_printerwrite(&data->pbuf, "0", 1);
		ft_putnbr_base_buffer(buf[(reader->text_offset & MASK_64_BITS) + i + j],
			BASE_HEX, &data->pbuf);
		if (j % 4 == 3)
			ft_printerwrite(&data->pbuf, " ", 1);
		j++;
	}
	if (data->opts & SHOW_CHARACTERS)
		while (j < 16)
		{
			ft_printerwrite_str(&data->pbuf, "   ");
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
			ft_printerwrite(&data->pbuf, &buf[reader->text_offset + i + j], 1);
		else
			ft_printerwrite(&data->pbuf, ".", 1);
		j++;
	}
}

static void		print_header(t_nm_otool *data)
{
	if (data->print)
	{
		ft_printerwrite_str(&data->pbuf, data->filename);
		if (data->objname)
		{
			ft_printerwrite_str(&data->pbuf, "(");
			ft_printerwrite_str(&data->pbuf, data->objname);
			ft_printerwrite_str(&data->pbuf, ")");
		}
		ft_printerwrite_str(&data->pbuf, ":");
		ft_printerwrite_str(&data->pbuf, "\n");
	}
	ft_printerwrite_str(&data->pbuf, "Contents of (");
	ft_printerwrite_str(&data->pbuf, data->segment);
	ft_printerwrite_str(&data->pbuf, ",");
	ft_printerwrite_str(&data->pbuf, data->section);
	ft_printerwrite_str(&data->pbuf, ") section\n");
}

int				ft_otool_print_section(t_nm_otool *data, t_freader *reader,
					uint8_t *start, int is_ppc)
{
	size_t	i;

	i = 0;
	ft_printerinit(&data->pbuf, STDOUT_FILENO, 1024, 0);
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
			ft_printerwrite_str(&data->pbuf, " ");
			putchars(data, reader, start, i);
		}
		ft_printerwrite_str(&data->pbuf, "\n");
		i += 16;
	}
	ft_printerclose_flush(&data->pbuf);
	return (0);
}
