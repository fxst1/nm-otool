/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_get_section_32.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 17:00:25 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/11 15:28:44 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static int	get_section2(t_segment32_command *cmd, char *sectname,
								t_section_info *sect)
{
	size_t	i;
	size_t	n;

	i = 0;
	n = cmd->nsects;
	while (i < n)
	{
		if (ft_strncmp(sectname, cmd->sections[i].sectname, 16) == 0)
		{
			sect->size = cmd->sections[i].size;
			sect->offset = cmd->sections[i].offset;
			return (1);
		}
		i++;
	}
	return (0);
}


void		mach_get_section_32(t_mach32 data, char *segname, char *sectname,
								t_section_info *sect)
{
	size_t				i;
	size_t				n;

	i = 0;
	n = data.header.ncmds;
	sect->size = 0;
	sect->offset = 0;
	while (i < n)
	{
		if (ft_strncmp(segname, data.cmds[i].segname, 16) == 0
			&& get_section2(&data.cmds[i], sectname, sect))
			break ;
		i++;
	}
}
