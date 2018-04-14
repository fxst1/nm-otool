/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftmach.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 17:30:22 by fjacquem         ###   ########.fr       */
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
#  define LC_SEGMENT 0x1
#  define LC_SYMTAB 0x2
#  define LC_SEGMENT_64 0x19
#  define LC_DYLD_INFO_ONLY 0x7FFFFFDE
# endif

typedef struct				s_mach32_section
{
	char					sectname[16];
	char					segname[16];
	uint32_t				addr;
	uint32_t				size;
	uint32_t				offset;
	uint32_t				align;
	uint32_t				reloff;
	uint32_t				nreloc;
	uint32_t				flags;
	uint32_t				reserved1;
	uint32_t				reserved2;
}							t_mach32_section;

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
	t_mach32_section		*sections;
}							t_segment32_command;

typedef struct				s_mach64_section
{
	char					sectname[16];
	char					segname[16];
	uint64_t				addr;
	uint64_t				size;
	uint32_t				offset;
	uint32_t				align;
	uint32_t				reloff;
	uint32_t				nreloc;
	uint32_t				flags;
	uint32_t				reserved1;
	uint32_t				reserved2;
}							t_mach64_section;

typedef struct				s_mach64_header
{
	uint32_t				magic;
	uint32_t				cputype;
	uint32_t				cpusubtype;
	uint32_t				filetype;
	uint32_t				ncmds;
	uint32_t				sizeofcmds;
	uint32_t				flags;
	uint32_t				reserved;
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
	t_mach64_section		*sections;
}							t_segment64_command;

typedef struct				s_symtab_command
{
	uint32_t				cmd;
	uint32_t				cmdsize;
	uint32_t				symoff;
	uint32_t				nsyms;
	uint32_t				stroff;
	uint32_t				strsize;
}							t_symtab_command;

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

typedef struct				s_list32
{
	uint32_t				strx;
	uint8_t					type;
	uint8_t					sect;
	int16_t					desc;
	uint32_t				value;
}							t_nlist32;

typedef struct				s_list64
{
	uint32_t				strx;
	uint8_t					type;
	uint8_t					sect;
	int16_t					desc;
	uint64_t				value;
}							t_nlist64;

typedef struct				s_load_command
{
	uint32_t				cmd;
	uint32_t				cmdsize;
}							t_load_command;

#endif
