/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_list_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 10:36:56 by fxst1            ###   ########.fr       */
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

static void 		print_list(t_symb *list)
{
	static char		*str = "?U????????????tT???";
	size_t			i;
	t_symb			sym;
	char			c;

	i = 0;
	while (list[i].name)
	{
		sym = list[i];
		if (sym.type > 15)
		{
			i++;
			continue ;
		}
		if (sym.value == 0)
			ft_putstr_fd("                 ", 1);
		else
			printf("%016x ", sym.value);
		c = str[sym.type];
		if (c == 't' && sym.sect == 10)
			c = 'b';
		write(1, &c, 1);
		ft_putstr_fd(" ", 1);
		ft_putstr_fd(sym.name, 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
}

void				nm_list_all(t_nm *data)
{
	t_symb			*list;

	if (data->bin.type_id == TYPE_ID_MACH64)
		list = mach_get_symbol_list_64(&data->bin);
	else if (data->bin.type_id == TYPE_ID_MACH32)
		list = mach_get_symbol_list_64(&data->bin);
	else
		return ;

	sort_table(list);
	print_list(list);
	free(list);
}
