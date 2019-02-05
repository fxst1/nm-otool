/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:34:42 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 21:22:54 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void		set_symbol_char(t_symbol *s)
{
	s->character = '?';
	if ((s->type & N_TYPE) == N_SECT)
	{
		if (ft_strcmp(s->segname, "__bss") == 0)
			s->character = 'B';
		else if (ft_strcmp(s->segname, "__text") == 0)
			s->character = 'T';
		else if (ft_strcmp(s->segname, "__data") == 0)
			s->character = 'D';
		else
			s->character = 'S';
	}
	else if ((s->type & N_TYPE) == N_UNDF)
		s->character = (s->type & 0x10) ? 'C' : 'U';
	else if ((s->type & N_TYPE) == N_ABS)
		s->character = 'A';
	else if ((s->type & N_TYPE) == N_PBUD)
		s->character = 'U';
	else if ((s->type & N_TYPE) == N_INDR)
		s->character = 'I';
	else if (s->type & N_STAB)
		s->character = '-';
	if (!(s->type & N_EXT) && s->character != '~' && s->character != '-')
		s->character += 32;
}

int			compare_symbols(void *p1, void *p2)
{
	t_symbol	*a;
	t_symbol	*b;
	int			ret;

	a = (t_symbol*)p1;
	b = (t_symbol*)p2;
	ret = ft_strcmp(a->symbname, b->symbname);
	if (ret == 0)
		return (a->value - b->value);
	return (ret);
}

static void	iter_symbols_default2(t_printerbuffered *pbuf,
				char character, char *symbname)
{
	ft_printerwrite_str(pbuf, " ");
	ft_printerwrite(pbuf, &character, 1);
	ft_printerwrite_str(pbuf, " ");
	ft_printerwrite_str(pbuf, symbname);
}

void		iter_symbols_default(void *nbits, t_list *lst)
{
	t_symbol			*s;
	uint64_t			value;
	t_printerbuffered	pbuf;

	ft_printerinit(&pbuf, STDOUT_FILENO, 1024, 0);
	s = (t_symbol*)lst->content;
	value = s->value;
	if ((size_t)nbits == 8)
		value = s->value & 0xffffffff;
	if (s->type & N_STAB)
		return ;
	set_symbol_char(s);
	if (value == 0 && s->character != 'T' && s->character != 't' &&
		s->character != 'a' && s->character != 'A')
		ft_printerwrite_str(&pbuf,
			(size_t)nbits == 8 ? "        " : "                ");
	else
		ft_putnbr_base_offset_buffer(value, BASE_HEX, (size_t)nbits,
			&pbuf);
	iter_symbols_default2(&pbuf, s->character, s->symbname);
	ft_printerwrite(&pbuf, "\n", 1);
	ft_printerclose_flush(&pbuf);
}
