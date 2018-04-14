/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_get_symbol_list_64.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 20:52:01 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/15 15:26:03 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void		elf_symbol2symb(t_binary *bin, t_symb **list, size_t offset,
								char *strtab)
{
	t_symb			sym;
	t_elf64_symbol	in;

	binary_is_corrupt(bin, bin->buffer + offset, sizeof(in));
	ft_memcpy(&in, bin->buffer + offset, sizeof(in));
	binary_strtab_corrupt(bin, strtab + in.name);
	sym.name = strtab + in.name + 1;
	sym.type = in.info & 0xf;
	sym.value = in.value;
	sym.sect = 0;
	printf("      at %lx, %d, %d\n", offset, SHT_SYMTAB, SHT_STRTAB);
	printf("    name %s (%x - %p)\n", strtab + in.name + 1, in.name, strtab);
	printf("   value %lx\n", in.value);
	printf("    size %lx\n", in.size);
	printf("   shndx %x\n\n", in.shndx);
	**list = sym;
	(*list)++;
}

static t_symb	*alloc_symbols(t_binary *bin, size_t n)
{
	size_t			i;
	size_t			size;
	t_symb			*symbs;
	t_elf64_section	*sections;

	i = 0;
	size = 0;
	sections = bin->content.elf64.sections;
	while (i < n)
	{
		if (sections[i].type == ELF_SHT_SYMTAB)
			size += (sections[i].size / sections[i].entsize);
		i++;
	}
	printf("size = %zu\n", size);
	symbs = (t_symb*)malloc(sizeof(t_symb) * (size + 1));
	ft_bzero(symbs, sizeof(t_symb) * (size + 1));
	if (!symbs)
	{
		binary_delete(bin);
		ft_putstr_fd("Cannot allocate symbols\n", 1);
		exit(EXIT_FAILURE);
	}
	return (symbs);
}

t_symb			*elf_get_symbol_list_64(t_binary *bin)
{
	t_symb	*list;
	size_t	i;
	size_t	j;
	size_t	n;
	size_t	offset;

	i = 0;
	n = bin->content.elf64.header.shnum;
	bin->symbols = alloc_symbols(bin, bin->content.elf64.header.shnum);
	list = bin->symbols;
	while (i < n)
	{
		if (bin->content.elf64.sections[i].type == ELF_SHT_SYMTAB)
		{
			j = 0;
			offset = bin->content.elf64.sections[i].offset;
			while (j < bin->content.elf64.sections[i].size)
			{
				elf_symbol2symb(bin, &list,
						offset + (j * bin->content.elf64.sections[i].entsize),
						bin->content.elf64.strtab);
				j += bin->content.elf64.sections[i].entsize;
			}
		}
		i++;
	}
	return (bin->symbols);
}
