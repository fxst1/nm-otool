/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_list_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 17:03:52 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

static void			sort_table(t_symb *in)
{
	size_t			i;
	size_t			j;
	t_symb			swap;

	i = 0;
	while (in[i].name)
	{
		j = i + 1;
		while (in[j].name)
		{
			if (ft_strcmp(in[i].name, in[j].name) > 0)
			{
				swap = in[i];
				in[i] = in[j];
				in[j] = swap;
			}
			j++;
		}
		i++;
	}
}

static void			print_symbol(t_symb sym)
{
	static char		*str = "?U????????????tT???";
	char			c;

	sym = list[i];
	if (sym.type > 15)
	{
		i++;
		continue ;
	}
	if (sym.value == 0)
		ft_putstr_fd(nbits == 8 ? "        " : "                ", 1);
	else
		ft_putnbr_base_offset_fd(sym.value, BASE_HEX, nbits, 1);
	c = str[sym.type];
	if (c == 't' && (sym.sect == 10 || sym.sect == 3))
		c = 'b';
	else if (c == 'T' && (sym.sect == 4))
		c = 'S';
	write(1, " ", 1);
	write(1, &c, 1);
	write(1, " ", 1);
	ft_putstr_fd(sym.name, 1);
	write(1, "\n", 1);
}

static void			print_list(t_symb *list, size_t nbits)
{
	size_t			i;

	i = 0;
	while (list[i].name)
	{
		print_symbol(list[i]);
		i++;
	}
}

void				nm_list_all(t_nm *data)
{
	size_t			nbits;

	if (data->bin.type_id == TYPE_ID_MACH64)
	{
		data->bin.symbols = mach_get_symbol_list_64(&data->bin);
		nbits = 16;
	}
	else if (data->bin.type_id == TYPE_ID_MACH32)
	{
		data->bin.symbols = mach_get_symbol_list_32(&data->bin);
		nbits = 8;
	}
	else
		return ;
	sort_table(data->bin.symbols);
	print_list(data->bin.symbols, nbits);
}
