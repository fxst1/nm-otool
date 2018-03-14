#include <binary.h>

static size_t	read_sections(uint8_t *buf, t_segment64_command *cmd,
								t_binary *bin)
{
	size_t				i;

	i = 0;
	cmd->sections = (t_mach64_section*)malloc(sizeof(t_mach64_section) *
												(cmd->nsects + 1));
	if (!cmd->sections)
	{
		ft_putstr_fd("Cannot allocate sections\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	ft_bzero(cmd->sections, sizeof(t_mach64_section) * (cmd->nsects + 1));
	while (i < cmd->nsects)
	{
		binary_is_corrupt(bin, buf, sizeof(t_mach64_section));
		ft_memcpy(&cmd->sections[i], buf, sizeof(t_mach64_section));
		buf += sizeof(t_mach64_section);
		i++;
	}
	return (0);
}

int				mach_read_64(t_binary *h)
{
	size_t					i;
	size_t					n;
	uint8_t					*buf;
	t_segment64_command		seg;

	i = 0;
	buf = h->buffer;
	h->type_id = TYPE_ID_MACH64;
	ft_memcpy(&h->content.mach64.header, buf, sizeof(t_mach64_header));
	buf += sizeof(t_mach64_header);
	n = (size_t)h->content.mach64.header.ncmds;
	h->content.mach64.cmds = (t_segment64_command*)malloc(
										sizeof(t_segment64_command) * (n + 1));
	if (!h->content.mach64.cmds)
		return (-1);
	ft_bzero(h->content.mach64.cmds, sizeof(t_segment64_command) * (n + 1));
	while (i < n)
	{
		binary_is_corrupt(h, buf, sizeof(t_segment64_command));
		ft_memcpy(&seg, buf, sizeof(t_segment64_command));
		if (seg.cmd == LC_SEGMENT_64)
			read_sections(buf + sizeof(t_segment64_command)
								- sizeof(t_mach64_section*), &seg, h);
		else
			seg.sections = NULL;
		h->content.mach64.cmds[i] = seg;
		buf += seg.cmdsize ? seg.cmdsize : sizeof(t_segment64_command);
		i++;
	}
	return (0);
}
