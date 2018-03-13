#include <otool.h>

static void	show_mach64_header(t_mach64 *data)
{
	ft_putstr_fd("Mach header\n", 1);
	ft_putstr_fd(" magic       ", 1);
	ft_putstr_fd(" cputype", 1);
	ft_putstr_fd(" cpusubtype", 1);
	ft_putstr_fd(" caps", 1);
	ft_putstr_fd(" filetype", 1);
	ft_putstr_fd(" ncmds", 1);
	ft_putstr_fd(" sizeofcmds", 1);
	ft_putstr_fd(" flags\n ", 1);
	ft_puthex_fd(data->header.magic, 1);
	ft_putstr_fd("\t", 1);
	ft_putnbr_fd(data->header.cputype, 1);
	ft_putstr_fd("\t", 1);
	ft_putnbr_fd(data->header.cpusubtype & 0xFFFFFF, 1);
	ft_putstr_fd("\t", 1);
	ft_puthex_fd((data->header.cpusubtype >> 24), 1);
	ft_putstr_fd("\t", 1);
	ft_putnbr_fd(data->header.filetype, 1);
	ft_putstr_fd("\t", 1);
	ft_putnbr_fd(data->header.ncmds, 1);
	ft_putstr_fd("\t", 1);
	ft_putnbr_fd(data->header.sizeofcmds, 1);
	ft_putstr_fd("\t", 1);
	ft_puthex_fd(data->header.flags, 1);
	ft_putstr_fd("\n", 1);
}

static void	show_mach32_header(t_mach32 *data)
{
	ft_putstr_fd("Mach header\n", 1);
	ft_putstr_fd("      magic", 1);
	ft_putstr_fd(" cputype", 1);
	ft_putstr_fd(" cpusubtype", 1);
	ft_putstr_fd(" caps", 1);
	ft_putstr_fd(" filetype", 1);
	ft_putstr_fd(" ncmds", 1);
	ft_putstr_fd(" sizeofcmds", 1);
	ft_putstr_fd("      flags\n ", 1);
	ft_puthex_fd(data->header.magic, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(data->header.cputype, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(data->header.cpusubtype, 1);
	ft_putstr_fd("      ", 1);
	ft_puthex_fd(data->header.filetype, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(data->header.ncmds, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(data->header.sizeofcmds, 1);
	ft_putstr_fd(" ", 1);
	ft_putnbr_fd(data->header.flags, 1);
	ft_putstr_fd("\n", 1);
}

void		otool_header(t_otool *data)
{
	if (data->bin.type_id == TYPE_ID_MACH32)
		show_mach32_header(&data->bin.content.mach32);
	else if (data->bin.type_id == TYPE_ID_MACH64)
		show_mach64_header(&data->bin.content.mach64);
	else if (data->bin.type_id == TYPE_ID_ELF32 ||
			data->bin.type_id == TYPE_ID_ELF64)
		ft_putstr_fd("Elf header\n", 1);
	else
		ft_putstr_fd("undefined format\n", 1);
	(void)data;
}
