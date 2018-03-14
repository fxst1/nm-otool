/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_get_symbol_list_64.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 13:03:54 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void			mach_get_symbols_64(uint8_t *buf, t_symtab_command *sym,
										t_symb **list, t_binary *bin)
{
	size_t			i;
	size_t			n;
	char			*strtab;
	t_nlist64		*symb;
	t_symb			msymb;

	i = 0;
	n = sym->nsyms;
	strtab = (char*)(buf + sym->stroff);
	symb = (t_nlist64*)(buf + sym->symoff);
	while (i < n)
	{
		binary_strtab_corrupt(bin, strtab + symb->strx);
		binary_is_corrupt(bin, symb, sizeof(t_nlist64));
		msymb.value = symb->value;
		msymb.type = symb->type;
		msymb.sect = symb->sect;
		msymb.name = strtab + symb->strx;
		**list = msymb;
		(*list)++;
		i++;
		symb++;
	}
}

static size_t				get_size(t_segment64_command *cmd)
{
	t_symtab_command		sym;

	ft_memcpy(&sym, cmd, sizeof(sym));
	return (sym.nsyms);
}

static t_symb				*alloc_symbols(t_binary *bin, size_t size)
{
	size_t					i;
	size_t 					n;
	t_segment64_command		cmd;

	i = 0;
	n = bin->content.mach64.header.ncmds;
	size = 0;
	while (i < n)
	{
		cmd = bin->content.mach64.cmds[i];
		if (cmd.cmd == LC_SYMTAB)
			size += get_size(&cmd);
		i++;
	}
	bin->symbols = (t_symb*)malloc(sizeof(t_symb) * (size + 1));
	if (bin->symbols == NULL)
	{
		binary_delete(bin);
		ft_putstr_fd("Cannot allocate symbol list\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(bin->symbols, sizeof(t_symb) * (size + 1));
	return (bin->symbols);
}

t_symb						*mach_get_symbol_list_64(t_binary *bin)
{
	size_t					i;
	size_t 					n;
	t_symtab_command		sym;
	t_symb					*list;
	t_symb					*tmp;

	i = 0;
	n = bin->content.mach64.header.ncmds;
	list = alloc_symbols(bin, 0);
	tmp = list;
	while (i < n)
	{
		if (bin->content.mach64.cmds[i].cmd == LC_SYMTAB)
		{
			ft_memcpy(&sym, &bin->content.mach64.cmds[i], sizeof(sym));
			mach_get_symbols_64(bin->buffer, &sym, &tmp, bin);
		}
		i++;
	}
	return (list);
}
