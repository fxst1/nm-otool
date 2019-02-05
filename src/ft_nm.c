/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 14:12:21 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 17:11:37 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

void			ft_nm_print(t_nm_otool *data)
{
	if (data->show_filename)
	{
		write(STDOUT_FILENO, "\n", 1);
		ft_putstr(data->filename);
		write(STDOUT_FILENO, ":\n", 2);
	}
	ft_lstsort(&data->symbols, &compare_symbols);
	if (data->opts == 0)
		ft_lstiter2(data->symbols, &iter_symbols_default,
			(void*)(uintptr_t)data->nbits);
}

void			ft_nm_clear(t_nm_otool *data)
{
	t_list		*next;
	t_list		*tmp;

	next = NULL;
	tmp = data->symbols;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->content);
		free(tmp);
		tmp = next;
	}
	data->symbols = NULL;
}

static int		parse_magic(t_nm_otool *data, uint8_t *buf, uint32_t magic)
{
	if (magic == MH_MAGIC_64)
	{
		data->nbits = 16;
		return (ft_nm_macho64(data, buf));
	}
	else if (magic == MH_MAGIC || magic == MH_CIGAM)
	{
		data->nbits = 8;
		return (magic == MH_CIGAM ? ft_nm_ppc(data, buf) :
									ft_nm_macho32(data, buf));
	}
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		return (ft_nm_fat(data, buf, magic == FAT_CIGAM));
	else if (ft_strncmp(ARCH_SIGN, (char*)buf, ARCH_SIGN_LEN) == 0)
	{
		data->print = 0;
		return (ft_nm_ar(data, buf));
	}
	else
		display_error(data, -1, "Undefined magic number\n");
	return (1);
}

int				ft_nm(t_nm_otool *data, uint8_t *buf)
{
	if (binary_is_corrupt(data, buf, 8))
		corruption_error(data, "magic\n");
	else
		parse_magic(data, buf, *(uint32_t*)buf);
	if (data->print)
	{
		ft_nm_print(data);
		ft_nm_clear(data);
	}
	return (0);
}
