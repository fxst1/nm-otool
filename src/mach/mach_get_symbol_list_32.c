/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_get_symbol_list_32.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 09:52:59 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void			mach_get_symbols_32(uint8_t *buf, t_symtab_command *sym,
										t_symb **list)
{
	size_t			i;
	size_t			n;
	char			*strtab;
	t_nlist32		*symb;
	t_symb			msymb;

	i = 0;
	n = sym->nsyms;
	strtab = (char*)(buf + sym->stroff);
	symb = (t_nlist32*)(buf + sym->symoff);
	while (i < n)
	{
		msymb.value = symb->value;
		msymb.type = symb->type;
		msymb.name = strtab + symb->strx;
		**list = msymb;
		(*list)++;
		i++;
		symb++;
	}
}

static size_t				get_size(t_segment32_command *cmd)
{
	t_symtab_command		sym;

	ft_memcpy(&sym, cmd, sizeof(sym));
	return (sym.nsyms);
}

static t_symb				*alloc_symbols(t_binary *bin, size_t size)
{
	size_t					i;
	size_t 					n;
	t_segment32_command		cmd;
	t_symb					*list;

	i = 0;
	n = bin->content.mach32.header.ncmds;
	size = 0;
	while (i < n)
	{
		cmd = bin->content.mach32.cmds[i];
		if (cmd.cmd == LC_SYMTAB)
			size += get_size(&cmd);
		i++;
	}
	list = (t_symb*)malloc(sizeof(t_symb) * (size + 1));
	if (list == NULL)
	{
		ft_putstr_fd("Cannot allocate symbol list\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(list, sizeof(t_symb) * (size + 1));
	return (list);
}

t_symb						*mach_get_symbol_list_32(t_binary *bin)
{
	size_t					i;
	size_t 					n;
	t_symtab_command		sym;
	t_symb					*list;
	t_symb					*tmp;

	i = 0;
	n = bin->content.mach32.header.ncmds;
	list = alloc_symbols(bin, 0);
	tmp = list;
	while (i < n)
	{
		if (bin->content.mach32.cmds[i].cmd == LC_SYMTAB)
		{
			ft_memcpy(&sym, &bin->content.mach32.cmds[i], sizeof(sym));
			mach_get_symbols_32(bin->buffer, &sym, &tmp);
		}
		i++;
	}
	return (list);
}
