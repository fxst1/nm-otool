#include <otool.h>

void			otool_text(t_otool *data)
{
	t_section_info	sect;
	size_t			i;
	size_t			j;

	printf("Sizeof mach64 header %zu\n", sizeof(t_mach64_header));
	get_segment_section(&data->bin, "__TEXT", "__text", &sect);
	if (sect.size > 0)
	{
		ft_putstr_fd("(__TEXT,__text) section\n", 1);
		printf("%ld, %x\n", sect.size, (uint32_t)sect.offset);
		printf("%zu\n", data->bin.size);
		i = 0;
		while (i < sect.size)
		{
			j = 0;
			ft_puthex_fd(sect.offset + i, 1);
			while (j < 16 && i + j < sect.size)
			{
				write(1, " ", 1);
				if (data->bin.buffer[sect.offset + i + j] <= 0xf)
					ft_putstr_fd("0", 1);
				ft_putnbr_base_fd(data->bin.buffer[sect.offset + i + j], BASE_HEX, 1);
				j++;
			}
			write(1, "\n", 1);
			i += 16;
		}
	}
}
