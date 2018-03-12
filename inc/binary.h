/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:00 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 11:29:26 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINARY_H
# define BINARY_H
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
# define TYPE_ID_MACH32 1
# define TYPE_ID_MACH64 2
# include <ftelf.h>
# include <ftmach.h>

typedef struct stat	t_stat;

typedef union		u_binary_type
{
	t_mach64		mach64;
	t_mach32		mach32;
	t_elf_header	elf;
}					t_binary_type;

typedef struct		s_binary
{
	uint8_t			type_id;
	uint32_t		magic;
	t_binary_type	content;
	uint8_t			*buffer;
	size_t			size;
}					t_binary;

int					binary_read(const char *filename, t_binary *h);
int					mach_read_32(t_binary *h);
int					mach_read_64(t_binary *h);

void				ft_memcpy(void *dst, void *src, size_t len);
void				ft_bzero(void *dst, size_t len);
size_t				ft_strlen(const char *s);
void				ft_putstr_fd(const char *s, const int fd);

#endif
