/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftfat.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 13:30:18 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 20:10:41 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTFAT_H
# define FTFAT_H
# ifndef FAT_MAGIC
#  define FAT_CIGAM	0xBEBAFECA
# endif
# ifndef FAT_MAGIC
#  define FAT_MAGIC	0xCAFEBABE
# endif

struct s_binary;

typedef struct		s_fat_arch
{
	uint32_t		cputype;
	uint32_t		cpusubtype;
	uint32_t		offset;
	uint32_t		size;
	uint32_t		align;
}					t_fat_arch;

typedef struct		s_fat
{
	uint32_t		n_arch;
	t_fat_arch		*archs;
	struct s_binary	*contents;
}					t_fat;

#endif
