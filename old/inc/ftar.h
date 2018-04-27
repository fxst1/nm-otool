/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftar.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 13:30:18 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 16:45:04 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTAR_H
# define FTAR_H
# include <bintypes.h>
# define ARMAG "!<arch>\n"
# define SARMAG 8

/*
**	ARICHIVE HEADER
**
**	mtime	(struct timespec)
**	uid		(uid_t)
**	gid		(gid_t)
**	mode	(mode_t)
**	size	(off_t)
*/

struct s_binary;

typedef struct				s_ranlib
{
	uint32_t				strx;
	uint32_t				offset;
}							t_ranlib;

typedef struct				s_ar_header
{
	uint8_t					name[16];
	uint8_t					mtime[12];
	uint8_t					uid[6];
	uint8_t					gid[6];
	uint8_t					mode[8];
	uint8_t					size[10];
	uint8_t					end[2];
}							t_ar_header;

typedef struct				s_ar_object
{
	char					*name;
	char					*objname;
	t_ar_header				header;
	t_ranlib				info;
	uint8_t					type_id;
	struct s_binary			*bin;
}							t_ar_object;

typedef struct				s_ar
{
	t_ar_header				header;
	off_t					start_content;
	char					*strtab;
	t_ar_object				*contents;
	size_t					n_content;
}							t_ar;

#endif
