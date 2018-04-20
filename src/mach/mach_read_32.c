/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mach_read_32.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 13:28:04 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/20 20:07:06 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static size_t			read_sections(uint8_t *buf, t_segment32_command *cmd,
								t_binary *bin)
{
	size_t				i;

	i = 0;
	cmd->nsects = may_swap32(bin->swap, cmd->nsects);
	cmd->sections = (t_mach32_section*)malloc(sizeof(t_mach32_section) *
												(cmd->nsects + 1));
	if (!cmd->sections)
	{
		binary_delete(bin);
		ft_putstr_fd("Cannot allocate sections\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(cmd->sections, sizeof(t_mach32_section) * (cmd->nsects + 1));
	while (i < cmd->nsects)
	{
		binary_is_corrupt(bin, buf, sizeof(t_mach32_section));
		ft_memcpy(&cmd->sections[i], buf, sizeof(t_mach32_section));
		buf += sizeof(t_mach32_section);
		i++;
	}
	return (0);
}

static size_t			get_header(t_binary *h)
{
	size_t				n;

	binary_is_corrupt(h, h->buffer, sizeof(t_mach32_header));
	ft_memcpy(&h->content.mach32.header, h->buffer, sizeof(t_mach32_header));
	h->content.mach32.header.ncmds = may_swap32(h->swap,
										h->content.mach32.header.ncmds);
	n = h->content.mach32.header.ncmds;
	h->content.mach32.cmds = (t_segment32_command*)malloc(
										sizeof(t_segment32_command) * (n + 1));
	if (!h->content.mach32.cmds)
	{
		binary_delete(h);
		ft_putstr_fd("Cannot allocate segments", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(h->content.mach32.cmds, sizeof(t_segment32_command) * (n + 1));
	return (n);
}

int						mach_read_32(t_binary *h, int force_swap)
{
	size_t					i;
	size_t					n;
	uint8_t					*buf;
	t_segment32_command		seg;

	i = 0;
	h->swap = ((*(uint32_t*)h->buffer == MH_CIGAM));
	n = get_header(h);
	buf = h->buffer + sizeof(t_mach32_header);
	h->type_id = TYPE_ID_MACH32;
	while (i < n)
	{
		binary_is_corrupt(h, buf, sizeof(t_segment32_command));
		ft_memcpy(&seg, buf, sizeof(t_segment32_command));
		if (seg.cmd == LC_SEGMENT)
			read_sections(buf + sizeof(t_segment32_command)
								- sizeof(t_mach32_section*), &seg, h);
		else
			seg.sections = NULL;
		h->content.mach32.cmds[i] = seg;
		buf += seg.cmdsize ? may_swap32(h->swap, seg.cmdsize) :
													sizeof(t_segment32_command);
		i++;
	}
	(void)force_swap;
	return (0);
}
