/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:00 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/20 19:07:30 by fjacquem         ###   ########.fr       */
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
#  include <mach-o/ranlib.h>
#  include <mach-o/fat.h>
#  include <ar.h>
#  define EI_NIDENT 16
# elif defined FT_LINUX
#  include <elf.h>
# elif defined FT_WIN
#  error "WINDOWS IS NOT IMPLEMENT YET"
# endif
# ifndef MAP_ANONYMOUS
#  define MAP_ANONYMOUS MAP_ANON
# endif
# define TYPE_ID_MACH32 1
# define TYPE_ID_MACH64 2
# define TYPE_ID_ELF64 3
# define TYPE_ID_ELF32 4
# define TYPE_ID_FAT 5
# define TYPE_ID_RANLIB 6
# include <bintypes.h>

typedef struct stat	t_stat;

typedef struct		s_section_info
{
	size_t			size;
	off_t			offset;
}					t_section_info;

typedef struct		s_symb
{
	char			*name;
	int				type;
	char			type_char;
	int				sect;
	uint64_t		value;
}					t_symb;

typedef struct		s_binary
{
	uint8_t			swap;
	uint8_t			type_id;
	uint32_t		magic;
	t_binary_type	content;
	uint8_t			*buffer;
	uint8_t			*ptr;
	size_t			size;
	t_symb			*symbols;
	size_t			n_symbols;
}					t_binary;

uint32_t			may_swap32(int swap, uint32_t value);

void				list_all_symbols(t_binary *data, char *path);
void				list_archive(t_binary *data, char *path);
void				list_fat(t_binary *data);

void				binary_strtab_corrupt(t_binary *bin, char *addr);
void				binary_is_corrupt(t_binary *bin, void *addr, size_t nbytes);

int					binary_read(const char *filename, t_binary *h);
int					binary_parse(t_binary *h, int force_swap);
void				binary_delete(t_binary *bin);

void				mach_clear_32(t_mach32 mach);
void				mach_clear_64(t_mach64 mach);

int					mach_read_32(t_binary *bin, int force_swap);
int					mach_read_64(t_binary *bin, int force_swap);
int					fat_read_64(t_binary *bin);
int					ar_read_64(t_binary *bin);

char				*ar_get_name(t_ar_header *hobj);

char				sections_char(char **names);
char				**mach_get_n_section_name(t_binary *bin, size_t section_index);

t_symb				*mach_get_symbol_list_64(t_binary *bin);
t_symb				*mach_get_symbol_list_32(t_binary *bin);
t_symb				*fat_get_symbol_list(t_binary *bin);

void				get_segment_section(t_binary *bin, char *segname,
										char *sectname, t_section_info *sect);
void				mach_get_section_32(t_mach32 data, char *segname,
										char *sectname, t_section_info *sect);
void				mach_get_section_64(t_mach64 data, char *segname,
										char *sectname, t_section_info *sect);
void				elf_get_section_64(t_binary *bin, char *sectname,
										t_section_info *sect);
#endif
