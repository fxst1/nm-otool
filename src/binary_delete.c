/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_delete.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 11:12:26 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 16:05:39 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void		binary_delete(t_binary *bin)
{
	if (bin->type_id == TYPE_ID_MACH64)
		mach_clear_64(bin->content.mach64);
	else if (bin->type_id == TYPE_ID_MACH32)
		mach_clear_32(bin->content.mach32);
	else if (bin->type_id == TYPE_ID_ELF64)
		elf_clear_64(bin->content.elf64);
	else if (bin->type_id == TYPE_ID_ELF32)
		elf_clear_32(bin->content.elf32);
	free(bin->buffer);
	free(bin->symbols);
}
