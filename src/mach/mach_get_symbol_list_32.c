/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_get_symbol_list_32.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/20 20:29:37 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void				syssymbol_to_symbol(t_nlist32 *syssymb, t_symb *symb,
							char **names)
{
	symb->value = syssymb->value;
	symb->type = syssymb->type;
	symb->sect = syssymb->sect;
	if (syssymb->type & N_STAB)
		symb->type_char = '~';
	else if ((syssymb->type & N_TYPE) == N_UNDF)
		symb->type_char = 'U';
	else if ((syssymb->type & N_TYPE) == N_ABS)
		symb->type_char = 'A';
	else if ((syssymb->type & N_TYPE) == N_INDR)
		symb->type_char = 'I';
	else if ((syssymb->type & N_TYPE) == N_PBUD)
		symb->type_char = 'U';
	else if ((syssymb->type & N_TYPE) == N_SECT)
		symb->type_char = sections_char(names);
	else
		symb->type_char = '?';
	if (!(syssymb->type & N_EXT) && symb->type_char != '?' &&
														symb->type_char != '~')
		symb->type_char += 32;
	free(names);
}

static void				mach_get_symbols_32(t_symtab_command *sym,
										t_symb **list, t_binary *bin)
{
	size_t				i;
	size_t				n;
	char				*strtab;
	t_nlist32			*symb;
	t_symb				msymb;

	i = 0;
	n = sym->nsyms;
	strtab = (char*)(bin->buffer + may_swap32(bin->swap, sym->stroff));
	symb = (t_nlist32*)(bin->buffer + may_swap32(bin->swap, sym->stroff));
	while (i < n)
	{
		binary_strtab_corrupt(bin, strtab + may_swap32(bin->swap, symb->strx));
		binary_is_corrupt(bin, symb, sizeof(t_nlist64));
		syssymbol_to_symbol(symb, &msymb,
				mach_get_n_section_name(bin, symb->sect));
		msymb.name = strtab + may_swap32(bin->swap, symb->strx);
		**list = msymb;
		(*list)++;
		i++;
		symb++;
	}
}

static size_t			get_size(t_segment32_command *cmd)
{
	t_symtab_command	sym;

	ft_memcpy(&sym, cmd, sizeof(sym));
	return (sym.nsyms);
}

static t_symb			*alloc_symbols(t_binary *bin, size_t size)
{
	size_t				i;
	size_t				n;
	t_segment32_command	cmd;

	i = 0;
	n = bin->content.mach32.header.ncmds;
	size = 0;
	while (i < n)
	{
		cmd = bin->content.mach32.cmds[i];
		if (may_swap32(bin->swap, cmd.cmd) == LC_SYMTAB)
			size += may_swap32(bin->swap, get_size(&cmd));
		i++;
	}
	bin->n_symbols = size;
	bin->symbols = (t_symb*)malloc(sizeof(t_symb) * size);
	if (bin->symbols == NULL)
	{
		binary_delete(bin);
		ft_putstr_fd("Cannot allocate symbol list\n", 2);
		exit(EXIT_FAILURE);
	}
	ft_bzero(bin->symbols, sizeof(t_symb) * size);
	return (bin->symbols);
}

t_symb					*mach_get_symbol_list_32(t_binary *bin)
{
	size_t				i;
	size_t				n;
	t_symtab_command	sym;
	t_symb				*list;
	t_symb				*tmp;

	i = 0;
	n = bin->content.mach32.header.ncmds;
	list = alloc_symbols(bin, 0);
	tmp = list;
	while (i < n)
	{
		if (bin->content.mach32.cmds[i].cmd == LC_SYMTAB)
		{
			ft_memcpy(&sym, &bin->content.mach32.cmds[i], sizeof(sym));
			mach_get_symbols_32(&sym, &tmp, bin);
		}
		i++;
	}
	return (list);
}
