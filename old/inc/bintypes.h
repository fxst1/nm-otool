/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bintypes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 15:31:29 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 19:39:49 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BINTYPES_H
# define BINTYPES_H
# include <ftelf.h>
# include <ftmach.h>
# include <ftfat.h>
# include <ftar.h>

typedef union		u_binary_type
{
	t_mach64		mach64;
	t_mach32		mach32;
	t_elf64			elf64;
	t_elf32			elf32;
	t_fat			fat;
	t_ar			ar;
}					t_binary_type;

#endif
