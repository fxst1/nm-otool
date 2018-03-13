/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_list_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/13 12:47:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/13 13:36:13 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

static void			mach_print_symbols_64(uint8_t *buf, off_t offset, t_symtab_command *sym)
{
	size_t			i;
	size_t			n;

	i = 0;
	n = sym->nsyms;
	while (i < n)
	{
		if (*buf == ' ')
		{
			buf += 2;
			continue ;
		}
		printf("0x%08x %s\n", (uint32_t)offset, buf);
		offset += ft_strlen((char*)buf) + 1;
		buf += ft_strlen((char*)buf) + 1;
		i++;
	}
}

static void			mach_list_symbols_64(t_binary *bin)
{
	size_t					i;
	size_t 					n;
	t_segment64_command		cmd;
	t_symtab_command		sym;

	i = 0;
	n = bin->content.mach64.header.ncmds;
	while (i < n)
	{
		cmd = bin->content.mach64.cmds[i];
		if (cmd.cmd == LC_SYMTAB)
		{
			ft_memcpy(&sym, &cmd, sizeof(sym));
			printf("     cmd %u\n", sym.cmd);
			printf(" cmdsize %u (%zu)\n", sym.cmdsize, sizeof(t_symtab_command));
			printf("   nsyms %u\n", sym.nsyms);
			printf("  stroff %u\n", sym.stroff);
			printf(" strsize %u\n\n", sym.strsize);
			mach_print_symbols_64(bin->buffer + sym.stroff, sym.stroff, &sym);
		}
		i++;
	}
}

void				nm_list_all(t_nm *data)
{
	if (data->bin.type_id == TYPE_ID_MACH64)
		mach_list_symbols_64(&data->bin);
	//else if (data->bin.content.type_id == TYPE_ID_MACH32)
	//	nm_list_all_32();
}
