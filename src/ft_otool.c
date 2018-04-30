#include "ft_nm_otool.h"

static void		putline(t_nm_otool *data, t_freader *reader, uint8_t *buf, size_t i)
{
	size_t		j;

	j = 0;
	ft_putnbr_base_offset_fd(reader->text_offset + i, BASE_HEX, data->nbits,
		STDOUT_FILENO);
	while (j < 16 && i + j < reader->text_size)
	{
		write(STDOUT_FILENO, " ", 1);
		if (buf[reader->text_offset + i + j] <= 0xf)
			ft_putstr_fd("0", STDOUT_FILENO);
		ft_putnbr_base_fd(buf[reader->text_offset + i + j],
			BASE_HEX, STDOUT_FILENO);
		j++;
	}
	if (data->opts & SHOW_CHARACTERS)
		while (j < 16)
		{
			write(STDOUT_FILENO, "   ", 3);
			j++;
		}
}

static void		putchars(t_nm_otool *data, t_freader *reader, uint8_t *buf, size_t i)
{
	size_t		j;

	j = 0;
	while (j < 16 && i + j < reader->text_size)
	{
		if (ft_isprint(buf[reader->text_offset + i + j]))
			write(STDOUT_FILENO, &buf[reader->text_offset + i + j], 1);
		else
			write(STDOUT_FILENO, ".", 1);
		j++;
	}
	(void)data;
}

int			ft_otool_print_section(t_nm_otool *data, t_freader *reader, uint8_t *start)
{
	size_t	i;

	i = 0;
	write(STDOUT_FILENO, "(", 1);
	ft_putstr_fd(data->segment, STDOUT_FILENO);
	write(STDOUT_FILENO, ",", 1);
	ft_putstr_fd(data->section, STDOUT_FILENO);
	ft_putstr_fd(") section\n", STDOUT_FILENO);
	while (i < reader->text_size)
	{
		putline(data, reader, start, i);
		if (data->opts & SHOW_CHARACTERS)
		{
			write(STDOUT_FILENO, " ", 1);
			putchars(data, reader, start, i);
		}
		write(STDOUT_FILENO, "\n", 1);
		i += 16;
	}
	return (0);
}


static int 		parse_magic(t_nm_otool *data, uint8_t *buf, uint32_t magic)
{
	if (magic == MH_MAGIC_64)
	{
		data->nbits = 16;
		return (ft_otool_macho64(data, buf));
	}
	else if (magic == MH_MAGIC)
	{
		data->nbits = 8;
	//	return (ft_otool_macho32(data, buf));
	}
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		;//return (ft_otool_fat(data, buf, magic == FAT_CIGAM));
	else if (ft_strncmp("!<arch>\n", (char*)buf, 8) == 0)
	{
		data->print = 0;
		;//return (ft_otool_ar(data, buf));
	}
	else
		display_error(data, -1, "Undefined MAGIC\n");
	return (1);
}

int 			ft_otool(t_nm_otool *data, uint8_t *buf)
{
	if (binary_is_corrupt(data, buf, 8))
		corruption_error(data, "Corrupt file: MAGIC\n");
	else
		parse_magic(data, buf, *(uint32_t*)buf);
	return (0);
}
