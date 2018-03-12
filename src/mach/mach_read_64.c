#include <binary.h>

int				mach_read_64(t_binary *h)
{
	size_t			i;
	size_t			n;
	uint8_t			*buf;

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
	while (i < h->content.mach64.header.ncmds)
	{
		ft_memcpy(&h->content.mach64.cmds[i], buf, sizeof(t_segment64_command));
		buf += sizeof(t_segment64_command);
		i++;
	}
	return (0);
}
