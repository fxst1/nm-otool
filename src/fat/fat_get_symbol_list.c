/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fat_get_symbol_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 21:37:29 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 22:02:54 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

static void	*my_realloc(void *old, void *add, size_t old_size, size_t new_size)
{
	void	*ret;
	size_t	i;

	i = 0;
	ret = malloc(new_size);
	ft_bzero(ret, new_size);
	ft_memcpy(ret, old, old_size);
	ft_memcpy(ret + old_size, add, new_size - old_size);
	free(old);
	return (ret);
}

t_symb		*fat_get_symbol_list(t_binary *bin)
{
	size_t	i;
	size_t	total;
	t_symb	*all;
	t_symb	*tmp;
	t_fat	fat;

	i = 0;
	total = 0;
	all = NULL;
	fat = bin->content.fat;
	printf("%p\n", &fat.n_arch);
	while (i < fat.n_arch)
	{
		if (fat.contents[i].type_id == TYPE_ID_MACH64)
			tmp = mach_get_symbol_list_64(&fat.contents[i]);
		else if (fat.contents[i].type_id == TYPE_ID_MACH32)
			tmp = mach_get_symbol_list_32(&fat.contents[i]);
		else
		{
			write(STDERR_FILENO, "Fat contains other format than mach-o\n", 38);
			exit(EXIT_FAILURE);
		}
		total += bin->n_symbols;
		all = my_realloc(all, tmp, bin->n_symbols, total);
		i++;
	}
	bin->n_symbols = total;
	return (all);
}
