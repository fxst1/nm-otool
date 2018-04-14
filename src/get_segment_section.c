#include <binary.h>

void		get_segment_section(t_binary *bin, char *segname, char *sectname,
								t_section_info *sect)
{
	if (bin->type_id == TYPE_ID_MACH32)
		mach_get_section_32(bin->content.mach32, segname, sectname, sect);
	else if (bin->type_id == TYPE_ID_MACH64)
		mach_get_section_64(bin->content.mach64, segname, sectname, sect);
	else if (bin->type_id == TYPE_ID_ELF64)
		elf_get_section_64(bin, sectname, sect);
}
