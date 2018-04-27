/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftelf.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:05 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 16:02:49 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTELF_H
# define FTELF_H
# include <sys/types.h>

typedef struct		s_elf32_header
{
	unsigned char	ident[EI_NIDENT];
	uint16_t		type;
	uint16_t		machine;
	uint32_t		version;
	uint32_t		entry;
	uint32_t		phoff;
	uint32_t		shoff;
	uint32_t		flags;
	uint16_t		ehsize;
	uint16_t		phentsize;
	uint16_t		phnum;
	uint16_t		shentsize;
	uint16_t		shnum;
	uint16_t		shstrndx;
}					t_elf32_header;

typedef struct		s_elf32_pheader
{
	uint32_t		p_type;
	uint32_t		p_offset;
	uint32_t		p_vaddr;
	uint32_t		p_paddr;
	uint32_t		p_filesz;
	uint32_t		p_memsz;
	uint32_t		p_flags;
	uint32_t		p_align;
}					t_elf32_pheader;

typedef struct		s_elf32
{
	t_elf32_header	header;
	t_elf32_pheader	pheader;
}					t_elf32;

typedef struct		s_elf64_header
{
	unsigned char	ident[EI_NIDENT];
	uint16_t		type;
	uint16_t		machine;
	uint32_t		version;
	uint64_t		entry;
	uint64_t		phoff;
	uint64_t		shoff;
	uint32_t		flags;
	uint16_t		ehsize;
	uint16_t		phentsize;
	uint16_t		phnum;
	uint16_t		shentsize;
	uint16_t		shnum;
	uint16_t		shstrndx;
}					t_elf64_header;

typedef struct		s_elf64_pheader
{
	uint32_t		p_type;
	uint32_t		p_offset;
	uint32_t		p_vaddr;
	uint32_t		p_paddr;
	uint32_t		p_filesz;
	uint32_t		p_memsz;
	uint32_t		p_flags;
	uint32_t		p_align;
}					t_elf64_pheader;

typedef struct		s_elf64
{
	t_elf64_header	header;
	t_elf64_pheader	pheader;
}					t_elf64;

#endif
