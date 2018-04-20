/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_segment_section.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/14 16:36:23 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 19:06:38 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void		get_segment_section(t_binary *bin, char *segname, char *sectname,
								t_section_info *sect)
{
	if (bin->type_id == TYPE_ID_MACH32)
		mach_get_section_32(bin->content.mach32, segname, sectname, sect);
	else if (bin->type_id == TYPE_ID_MACH64)
		mach_get_section_64(bin->content.mach64, segname, sectname, sect);
}
