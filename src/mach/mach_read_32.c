#include <binary.h>

int				mach_read_32(t_binary *h)
{
	size_t			i;
	size_t			n;
	uint8_t			*buf;

	i = 0;
	buf = h->buffer;
	h->type_id = TYPE_ID_MACH32;
	ft_memcpy(&h->content.mach32.header, buf, sizeof(t_mach32_header));
	buf += sizeof(t_mach32_header);
	n = (size_t)h->content.mach32.header.ncmds;
	h->content.mach32.cmds = (t_segment32_command*)malloc(sizeof(t_segment32_command) * (n + 1));
	if (!h->content.mach32.cmds)
		return (-1);
	ft_bzero(h->content.mach32.cmds, sizeof(t_segment32_command) * (n + 1));
	while (i < h->content.mach32.header.ncmds)
	{
		ft_memcpy(&h->content.mach32.cmds[i], buf, sizeof(t_segment32_command));
		buf += sizeof(t_segment32_command);
		i++;
	}
	return (0);
}
