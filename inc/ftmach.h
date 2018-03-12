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
# ifndef MH_MAGIC
#  define MH_MAGIC 0xFEEDFACEu
#  define MH_CIGAM 0xCEFAEDFEu
#  define MH_MAGIC_64 0xFEEDFACFu
#  define MH_CIGAM_64 0xCFFAEDFEu
#  define FAT_MAGIC 0xCAFEBABEu
#  define FAT_CIGAM 0xBEBAFECAu
#  define FAT_MAGIC_64 0xCAFEBABFu
#  define FAT_CIGAM_64 0xBFBAFECAu
# endif

typedef struct				s_mach32_header
{
	uint32_t				magic;
	uint32_t				cputype;
	uint32_t				cpusubtype;
	uint32_t				filetype;
	uint32_t				ncmds;
	uint32_t				sizeofcmds;
	uint32_t				flags;
}							t_mach32_header;

typedef struct				s_segment32_command
{
	uint32_t				cmd;
	uint32_t				cmdsize;
	char					segname[16];
	uint32_t				vmaddr;
	uint32_t				vmsize;
	uint32_t				fileoff;
	uint32_t				filesize;
	uint32_t				maxprot;
	uint32_t				initprot;
	uint32_t				nsects;
	uint32_t				flags;
}							t_segment32_command;

typedef struct				s_mach64_header
{
	uint32_t				magic;
	uint32_t				cputype;
	uint32_t				cpusubtype;
	uint32_t				filetype;
	uint32_t				ncmds;
	uint32_t				sizeofcmds;
	uint32_t				flags;
}							t_mach64_header;

typedef struct				s_segment64_command
{
	uint32_t				cmd;
	uint32_t				cmdsize;
	char					segname[16];
	uint64_t				vmaddr;
	uint64_t				vmsize;
	uint64_t				fileoff;
	uint64_t				filesize;
	uint32_t				maxprot;
	uint32_t				initprot;
	uint32_t				nsects;
	uint32_t				flags;
}							t_segment64_command;

typedef struct				s_mach32
{
	int						swap;
	t_mach32_header			header;
	t_segment32_command		*cmds;
}							t_mach32;

typedef struct				s_mach64
{
	int						swap;
	t_mach64_header			header;
	t_segment64_command		*cmds;
}							t_mach64;

#endif
