/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmach.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 11:09:55 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTMACH_H
# define FTMACH_H
# include <sys/types.h>
# define MH_MAGIC 0xFEEDFACEu
# define MH_CIGAM 0xCEFAEDFEu
# define MH_MAGIC_64 0xFEEDFACFu
# define MH_CIGAM_64 0xCFFAEDFEu
# define FAT_MAGIC 0xCAFEBABEu
# define FAT_CIGAM 0xBEBAFECAu
# define FAT_MAGIC_64 0xCAFEBABFu
# define FAT_CIGAM_64 0xBFBAFECAu

typedef struct		s_mach_header
{
	uint32_t		magic;
	uint32_t		cputype;
	uint32_t		cpusubtype;
	uint32_t		filetype;
	uint32_t		ncmds;
	uint32_t		sizeofcmds;
	uint32_t		flags;
}					t_mach_header;

typedef struct		s_segment_command
{
	uint32_t		cmd;
	uint32_t		cmdsize;
	char			segname[16];
	uint32_t		vmaddr;
	uint32_t		vmsize;
	uint32_t		fileoff;
	uint32_t		filesize;
	uint32_t		maxprot;
	uint32_t		initprot;
	uint32_t		nsects;
	uint32_t		flags;
}					t_segment_command;

#endif
