/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_all_symbols.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/17 22:04:21 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

static void			sort_table(t_symb *in, size_t n_symbols)
{
	size_t			i;
	size_t			j;
	int				cmp;
	t_symb			swap;

	i = 0;
	while (i < n_symbols)
	{
		j = i + 1;
		while (j < n_symbols)
		{
			cmp = ft_strcmp(in[i].name, in[j].name);
			if (cmp > 0 || (cmp == 0 && in[i].value > in[j].value))
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

static void			print_symbol(t_symb sym, size_t nbits)
{
	if (sym.type_char == 'M')
		return ;
	if (sym.value == 0 && sym.type_char != 'T')
		ft_putstr_fd(nbits == 8 ? "        " : "                ", 1);
	else
		ft_putnbr_base_offset_fd(sym.value, BASE_HEX, nbits, 1);
	write(1, " ", 1);
	write(1, &sym.type_char, 1);
	write(1, " ", 1);
	ft_putstr_fd(sym.name, 1);
	write(1, "\n", 1);
}

static void			print_list(t_symb *list, size_t nbits, size_t n_symbols)
{
	size_t			i;

	i = 0;
	while (i < n_symbols)
	{
		print_symbol(list[i], nbits);
		i++;
	}
}

void				list_all_symbols(t_binary *data)
{
	size_t			nbits;

	nbits = 16;
	if (data->type_id == TYPE_ID_MACH64)
		data->symbols = mach_get_symbol_list_64(data);
	else if (data->type_id == TYPE_ID_MACH32)
	{
		data->symbols = mach_get_symbol_list_32(data);
		nbits = 8;
	}
	else if (data->type_id == TYPE_ID_RANLIB)
	{
		list_archive(data);
		return ;
	}
	else if (data->type_id == TYPE_ID_FAT)
	{
		printf("%p\n", &data->content.fat.n_arch);
		data->symbols = fat_get_symbol_list(data);
	}
	sort_table(data->symbols, data->n_symbols);
	print_list(data->symbols, nbits, data->n_symbols);
}
