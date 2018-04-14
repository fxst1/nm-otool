/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_read_64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:32 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 16:35:36 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void	read_section(t_binary *bin, t_elf64 *elf, uint8_t *buf, size_t i)
{
	binary_is_corrupt(bin, buf, sizeof(t_elf64_section));
	ft_memcpy(&elf->sections[i], buf, sizeof(t_elf64_section));
	if (i == elf->header.shstrndx && elf->sections[i].type == ELF_SHT_STRTAB)
		elf->shstrtab = (char*)bin->buffer + elf->sections[i].offset;
	else if (elf->sections[i].type == ELF_SHT_STRTAB)
		elf->strtab = (char*)bin->buffer + elf->sections[i].offset;
}

static void	read_sections(t_binary *bin, t_elf64 *elf)
{
	size_t	i;
	size_t	n;
	uint8_t	*buf;

	i = 0;
	n = elf->header.shnum;
	elf->sections = (t_elf64_section*)malloc(sizeof(t_elf64_section) * (n + 1));
	if (!elf->sections)
	{
		binary_delete(bin);
		ft_putstr_fd("Cannot allocate sections", 1);
		exit(EXIT_FAILURE);
	}
	ft_bzero(elf->sections, sizeof(t_elf64_section) * (n + 1));
	buf = bin->buffer + elf->header.shoff;
	elf->shstrtab = NULL;
	elf->strtab = NULL;
	while (i < n)
	{
		read_section(bin, elf, buf, i);
		buf += elf->header.shentsize;
		i++;
	}
}

int			elf_read_64(t_binary *bin)
{
	uint8_t	*buf;

	bin->type_id = TYPE_ID_ELF64;
	binary_is_corrupt(bin, bin->buffer, sizeof(t_elf64_header));
	ft_memcpy(&bin->content.elf64.header, bin->buffer, sizeof(t_elf64_header));
	buf = bin->buffer;
	if (bin->content.elf64.header.phoff > 0)
	{
		binary_is_corrupt(bin, buf + bin->content.elf64.header.phoff,
				bin->content.elf64.header.phoff + sizeof(t_elf64_prog));
		ft_memcpy(&bin->content.elf64.prog,
					buf + bin->content.elf64.header.phoff,
					sizeof(t_elf64_prog));
	}
	bin->content.elf64.shstrtab = NULL;
	if (bin->content.elf64.header.shoff > 0)
		read_sections(bin, &bin->content.elf64);
	else
		bin->content.elf64.sections = NULL;
	return (0);
}
