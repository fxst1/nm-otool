#include <binary.h>

static size_t	read_sections(uint8_t *buf, t_segment64_command *cmd)
{
	size_t				i;

	i = 0;
	cmd->sections = (t_mach64_section*)malloc(sizeof(t_mach64_section) *
												(cmd->nsects + 1));
	ft_bzero(cmd->sections, sizeof(t_mach64_section) * (cmd->nsects + 1));
	while (i < cmd->nsects)
	{
	//	printf("==> %zu / %u \n", i, cmd->nsects);
		ft_memcpy(&cmd->sections[i], buf, sizeof(t_mach64_section));
	/*	printf("   segname %s\n", cmd->sections[i].sectname);
		printf("  sectname %s\n", cmd->sections[i].segname);
		printf("      addr 0x%lx\n", cmd->sections[i].addr);
		printf("      size 0x%lx\n", cmd->sections[i].size);
		printf("    offset %d\n", cmd->sections[i].offset);
		printf("     align %d\n", cmd->sections[i].align);
		printf("    reloff %d\n", cmd->sections[i].reloff);
		printf("    nreloc %d\n", cmd->sections[i].nreloc);
		printf("     flags %d\n", cmd->sections[i].flags);
		printf(" reserved1 %d\n", cmd->sections[i].reserved1);
		printf(" reserved2 %d\n", cmd->sections[i].reserved2);*/
		buf += sizeof(t_mach64_section);
		i++;
		if ( i >= 10)
			exit(0);
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
	h->content.mach64.cmds = (t_segment64_command*)malloc(sizeof(t_segment64_command) * (n + 1));
	if (!h->content.mach64.cmds)
		return (-1);
	ft_bzero(h->content.mach64.cmds, sizeof(t_segment64_command) * (n + 1));
	while (i < n)
	{
		//printf("Load command %ld\n", i);
		ft_memcpy(&seg, buf, sizeof(t_segment64_command));
	//	printf("%d\n", seg.cmd);
		if (seg.cmd == LC_SEGMENT_64)
			read_sections(buf + sizeof(t_segment64_command) - sizeof(t_mach64_section*), &seg);
		else
			seg.sections = NULL;
		h->content.mach64.cmds[i] = seg;
		buf += seg.cmdsize;
		i++;
	}
	return (0);
}
