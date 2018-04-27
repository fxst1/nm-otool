/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_get_symbol_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 21:37:29 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 19:55:39 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static t_binary		*get_arch(t_fat *fat)
{
	size_t		i;

	i = 0;
	while (i < fat->n_arch)
	{
		if (fat->contents[i].type_id == TYPE_ID_MACH64)
			return (&fat->contents[i]);
		i++;
	}
	i = 0;
	while (i < fat->n_arch)
	{
		if (fat->contents[i].type_id == TYPE_ID_MACH32)
			return (&fat->contents[i]);
		else
			return (NULL);
		i++;
	}
	return (NULL);
}

t_symb				*fat_get_symbol_list(t_binary *bin)
{
	t_symb		*list;
	t_binary	*arch;

	list = NULL;
	arch = get_arch(&bin->content.fat);
	if (!arch)
	{
		write(STDOUT_FILENO, "Undefined architecture(s)\n", 26);
		return (NULL);
	}
	if (arch->type_id == TYPE_ID_MACH64)
		list = mach_get_symbol_list_64(arch);
	else if (arch->type_id == TYPE_ID_MACH32)
		list = mach_get_symbol_list_32(arch);
	if (list)
		bin->n_symbols = arch->n_symbols;
	return (list);
}
