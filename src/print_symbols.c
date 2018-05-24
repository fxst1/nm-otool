/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_symbols.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:34:42 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 14:35:10 by fjacquem         ###   ########.fr       */
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
		s->character = 'U';
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

void		iter_symbols_default(void *nbits, t_list *lst)
{
	t_symbol *s;

	s = (t_symbol*)lst->content;
	if (s->type & N_STAB)
		return ;
	set_symbol_char(s);
	if (s->value == 0 && s->character != 'T')
		ft_putstr_fd((size_t)nbits == 8 ? "        " : "                ",
			STDOUT_FILENO);
	else
		ft_putnbr_base_offset_fd(s->value, "0123456789abcdef", (size_t)nbits,
			STDOUT_FILENO);
	write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, &s->character, 1);
	write(STDOUT_FILENO, " ", 1);
	ft_putstr_fd(s->symbname, STDOUT_FILENO);
	write(STDOUT_FILENO, "\n", 1);
}
