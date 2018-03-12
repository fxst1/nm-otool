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
#  include <ftelf.h>
typedef mach_header	t_mach_header;
# elif defined FT_LINUX
#  include <elf.h>
#  include <ftmach.h>
#  include <ftelf.h>
# elif defined FT_WIN
#  error "WINDOWS IS NOT IMPLEMENT YET"
# endif

typedef struct stat	t_stat;

typedef union		u_binary_type
{
	t_mach_header	mach;
	t_elf_header	elf;
}					t_binary_type;

typedef struct		s_binary_header
{
	uint8_t			typeid;
	t_binary_type	type;
	uint32_t		magic;
	size_t			header_size;
}					t_binary_header;

typedef struct		s_binary
{
	t_binary_header	header;
	uint8_t			*buffer;
	size_t			size;
}					t_binary;

#endif
