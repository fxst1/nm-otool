/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_read_64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 13:35:25 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 22:04:02 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

int				fat_read_64(t_binary *bin)
{
	uint32_t	i;
	t_fat		fat;
	off_t		offset;

	i = 0;
	offset = 8;
	bin->type_id = TYPE_ID_FAT;
	bin->swap = (*(uint32_t*)bin->buffer == FAT_CIGAM);
	binary_is_corrupt(bin, bin->buffer, 8);
	fat.n_arch = may_swap32(bin->swap, *(uint32_t*)(bin->buffer + 4));
	fat.contents = (t_binary*)malloc(sizeof(t_binary) * fat.n_arch);
	fat.archs = (t_fat_arch*)malloc(sizeof(t_fat_arch) * fat.n_arch);
	while (i < fat.n_arch)
	{
		binary_is_corrupt(bin, bin->buffer + offset, sizeof(t_fat_arch));
		fat.archs[i] = *(t_fat_arch*)(bin->buffer + offset);
		fat.archs[i].offset = may_swap32(bin->swap, fat.archs[i].offset);
		ft_memcpy(&fat.contents[i], bin, sizeof(t_binary));
		fat.contents[i].buffer = bin->buffer + fat.archs[i].offset;
		binary_parse(&fat.contents[i]);
		i++;
		offset += sizeof(t_fat_arch);
	}
	printf("%p\n", &fat.n_arch);
	return (0);
}
