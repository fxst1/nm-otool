/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_macho64.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:07:26 by fjacquem          #+#    #+#             */
/*   Updated: 2018/09/10 22:25:29 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int				look_symtab_64(t_nm_otool *data, uint8_t *buf,
					t_freader *reader)
{
	if (binary_is_corrupt(data, buf + 8, 16))
		return (corruption_error(data, "LC_SYMTAB\n"));
	reader->symtab_offset = *(uint32_t*)(buf + 8);
	reader->n_symbols = *(uint32_t*)(buf + 12);
	reader->strtab_offset = *(uint32_t*)(buf + 16);
	reader->strtab_size = *(uint32_t*)(buf + 20);
	return (0);
}

int				load_symtab_64(t_nm_otool *data, t_freader *reader,
					uint8_t *buf)
{
	uint32_t	i;
	uint32_t	type;
	uint32_t	size;

	i = 0;
	while (i < reader->n_load_commands)
	{
		if (binary_is_corrupt(data, buf, 8))
			return (corruption_error(data, "Load Command\n"));
		else
		{
			type = *((uint32_t*)buf);
			size = *((uint32_t*)buf + 1);
			if (size == 0)
				return (corruption_error(data, "Command size is nul\n"));
			else if (binary_is_corrupt(data, buf, size))
				return (corruption_error(data, "Section\n"));
			else if (type == LC_SYMTAB)
				if (look_symtab_64(data, buf, reader) != 0)
					return (1);
			buf += size;
		}
		i++;
	}
	return (0);
}

static int		init_symbol(t_nm_otool *data, t_symbol *s, uint8_t *buf,
					char *strtab)
{
	ft_bzero(s, sizeof(*s));
	if (binary_is_corrupt(data, buf, 0x10))
		return (corruption_error(data, "Symbol not correctly defined\n"));
	s->symbname = (char*)strtab + *(uint32_t*)buf;
	if (binary_strtab_corrupt(data, s->symbname))
		return (corruption_error(data, "Symbol name truncated\n"));
	s->type = *(buf + 4);
	s->value = *(uint64_t*)(buf + 8);
	return (0);
}

int				load_symbols_64(t_nm_otool *data, t_freader *reader,
					uint8_t *buf, uint8_t *start)
{
	size_t		i;
	size_t		sect_index;
	t_symbol	s;

	i = 0;
	while (i < reader->n_symbols)
	{
		if (init_symbol(data, &s, buf, (char*)start + reader->strtab_offset))
			return (1);
		sect_index = *(buf + 5);
		if (sect_index > 0 &&
			get_seg_sect_name_64(start, sect_index, &s, reader->n_load_commands)
				!= 0)
			return (corruption_error(data, "section index not found\n"));
		if (!(s.type & N_STAB))
			ft_lstadd_sort(&data->symbols, ft_lstnew(&s, sizeof(s)),
			&compare_symbols);
		buf += 0x10;
		i++;
	}
	return (0);
}

int				ft_nm_macho64(t_nm_otool *data, uint8_t *buf)
{
	t_freader	reader;

	ft_bzero(&reader, sizeof(reader));
	if (binary_is_corrupt(data, buf, 24))
		return (corruption_error(data, "mach-o64 header\n"));
	else
	{
		reader.n_load_commands = *(uint32_t*)(buf + 0x10);
		reader.size_load_commands = *(uint32_t*)(buf + 0x14);
		if (load_symtab_64(data, &reader, buf + 0x20) != 0)
			return (1);
		if (binary_is_corrupt(data, buf + reader.strtab_offset,
				reader.strtab_size))
			return (corruption_error(data, "String Table\n"));
		if (load_symbols_64(data, &reader, buf + reader.symtab_offset, buf)
				!= 0)
			return (1);
	}
	return (0);
}
