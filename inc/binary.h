/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:00 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 09:35:05 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINARY_H
# define BINARY_H
# include <libft.h>
# include <os.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/mman.h>
# include <unistd.h>
# include <fcntl.h>
# ifdef FT_APPLE
#  include <mach-o/loader.h>
#  include <mach-o/swap.h>
#  define EI_NIDENT 16
# elif defined FT_LINUX
#  include <elf.h>
# elif defined FT_WIN
#  error "WINDOWS IS NOT IMPLEMENT YET"
# endif
# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
# endif

# define ELF_MAGIC_32 0x4c46457f
# define ELF_MAGIC_64 ELF_MAGIC_32
# define TYPE_ID_MACH32 1
# define TYPE_ID_MACH64 2
# define TYPE_ID_ELF64 3
# define TYPE_ID_ELF32 4
# include <ftelf.h>
# include <ftmach.h>
typedef struct stat	t_stat;

typedef union		u_binary_type
{
	t_mach64		mach64;
	t_mach32		mach32;
	t_elf64			elf64;
	t_elf32			elf32;
}					t_binary_type;

typedef struct		s_binary
{
	uint8_t			type_id;
	uint32_t		magic;
	t_binary_type	content;
	uint8_t			*buffer;
	size_t			size;
}					t_binary;

typedef struct		s_section_info
{
	size_t			size;
	off_t			offset;
}					t_section_info;

typedef struct		s_symb
{
	char			*name;
	int				type;
	uint32_t		value;
}					t_symb;

int					binary_read(const char *filename, t_binary *h);
int					mach_read_32(t_binary *bin);
int					mach_read_64(t_binary *bin);
int					elf_read_32(t_binary *bin);
int					elf_read_64(t_binary *bin);

t_symb				*mach_get_symbol_list_64(t_binary *bin);
t_symb				*mach_get_symbol_list_32(t_binary *bin);

void				get_segment_section(t_binary *bin, char *segname,
										char *sectname, t_section_info *sect);
void				mach_get_section_32(t_mach32 data, char *segname,
										char *sectname, t_section_info *sect);
void				mach_get_section_64(t_mach64 data, char *segname,
										char *sectname, t_section_info *sect);

#endif
