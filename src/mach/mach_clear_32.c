/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_clear_32.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 12:48:13 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 12:54:58 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void			mach_clear_32(t_mach32 mach)
{
	size_t		i;

	i = 0;
	while (i < mach.header.ncmds)
	{
		free(mach.cmds[i].sections);
		i++;
	}
	free(mach.cmds);
}
