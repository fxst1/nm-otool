/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftelf.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:05 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 11:00:10 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTELF_H
# define FTELF_H
# include <sys/types.h>

typedef struct		s_elf_header
{
	unsigned char	ident[EI_NIDENT];
	uint16_t		type;
	uint16_t		machine;
	uint32_t		version;
	intptr_t		entry;
	off_t			phoff;
	off_t			shoff;
	uint32_t		flags;
	uint16_t		ehsize;
	uint16_t		phentsize;
	uint16_t		phnum;
	uint16_t		shentsize;
	uint16_t		shnum;
	uint16_t		shstrndx;
}					t_elf_header;

#endif
