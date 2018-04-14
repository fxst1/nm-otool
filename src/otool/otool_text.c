#include <otool.h>

static void 	otool_text_darwin(t_otool *data)
{
	t_section_info	sect;
	size_t			i;
	size_t			j;

	get_segment_section(&data->bin, "__TEXT", "__text", &sect);
	if (sect.size > 0)
	{
		ft_putstr_fd("(__TEXT,__text) section\n", 1);
		i = 0;
		while (i < sect.size)
		{
			j = 0;
			ft_putstr_fd("0x", 1);
			ft_putnbr_base_offset_fd(sect.offset + i, BASE_HEX, 16, 1);
			while (j < 16 && i + j < sect.size)
			{
				write(1, " ", 1);
				if (data->bin.buffer[sect.offset + i + j] <= 0xf)
				ft_putstr_fd("0", 1);
				ft_putnbr_base_fd(data->bin.buffer[sect.offset + i + j],
					BASE_HEX, 1);
					j++;
			}
			write(1, "\n", 1);
			i += 16;
		}
	}
}

static void 	otool_text_linux(t_otool *data)
{
	t_section_info	sect;
	size_t			i;
	size_t			j;

	get_segment_section(&data->bin, NULL, ".text", &sect);
	if (sect.size > 0)
	{
		ft_putstr_fd("(.text) section\n", 1);
		i = 0;
		while (i < sect.size)
		{
			j = 0;
			ft_putstr_fd("0x", 1);
			ft_putnbr_base_offset_fd(sect.offset + i, BASE_HEX, 16, 1);
			while (j < 16 && i + j < sect.size)
			{
				write(1, " ", 1);
				if (data->bin.buffer[sect.offset + i + j] <= 0xf)
				ft_putstr_fd("0", 1);
				ft_putnbr_base_fd(data->bin.buffer[sect.offset + i + j],
					BASE_HEX, 1);
					j++;
			}
			write(1, "\n", 1);
			i += 16;
		}
	}
}

void			otool_text(t_otool *data)
{
	if (data->bin.type_id == TYPE_ID_MACH64 || data->bin.type_id == TYPE_ID_MACH32)
		otool_text_darwin(data);
	else
		otool_text_linux(data);
}
