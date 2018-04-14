/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftelf.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:05 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 16:03:12 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTELF_H
# define FTELF_H
# include <sys/types.h>
# define ELF_32 1
# define ELF_64 2
# define ELF_SHT_SYMTAB 2
# define ELF_SHT_STRTAB 3

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

typedef struct		s_elf32_prog
{
	uint32_t		type;
	uint32_t		offset;
	uint32_t		vaddr;
	uint32_t		paddr;
	uint32_t		filesz;
	uint32_t		memsz;
	uint32_t		flags;
	uint32_t		align;
}					t_elf32_prog;

typedef struct		s_elf32_section
{
	uint32_t		name;
	uint32_t		type;
	uint32_t		flags;
	uint32_t		addr;
	uint32_t		offset;
	uint32_t		size;
	uint32_t		link;
	uint32_t		info;
	uint32_t		addralign;
	uint32_t		entsize;
}					t_elf32_section;

typedef struct		s_elf32_symbol
{
	uint32_t		name;
	uint32_t		value;
	uint32_t		size;
	unsigned char	info;
	unsigned char	other;
	uint16_t		shndx;
}					t_elf32_symbol;

typedef struct		s_elf32
{
	t_elf32_header	header;
	t_elf32_prog	prog;
	t_elf32_section	*sections;
	char			*shstrtab;
	char			*strtab;
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

typedef struct		s_elf64_prog
{
	uint32_t		type;
	uint32_t		offset;
	uint32_t		vaddr;
	uint32_t		paddr;
	uint32_t		filesz;
	uint32_t		memsz;
	uint32_t		flags;
	uint32_t		align;
}					t_elf64_prog;

typedef struct		s_elf64_section
{
	uint32_t		name;
	uint32_t		type;
	uint32_t		flags;
	uint64_t		addr;
	uint64_t		offset;
	uint32_t		size;
	uint32_t		link;
	uint32_t		info;
	uint32_t		addralign;
	uint32_t		entsize;
}					t_elf64_section;

typedef struct		s_elf64_symbol
{
	uint32_t		name;
	unsigned char	info;
	unsigned char	other;
	uint16_t		shndx;
	uint64_t		value;
	uint64_t		size;
}					t_elf64_symbol;

typedef struct		s_elf64
{
	t_elf64_header	header;
	t_elf64_prog	prog;
	t_elf64_section	*sections;
	char			*shstrtab;
	char			*strtab;
}					t_elf64;

#endif
