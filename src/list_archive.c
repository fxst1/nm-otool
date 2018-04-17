/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_archive.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 19:24:26 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/17 20:10:58 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void				list_archive(t_binary *data)
{
	size_t			i;
	t_ar_object		obj;

	i = 0;
	while (i < data->content.ar.n_content)
	{
		obj = data->content.ar.contents[i];
		write(STDOUT_FILENO, obj.name, ft_strlen(obj.name));
		write(STDOUT_FILENO, ":\n", 2);
		list_all_symbols(obj.bin);
		i++;
	}
}
/*
void				list_fat(t_binary *data)
{
	size_t			i;
	t_fat			fat;

	fat = data->content.fat;
	i = 0;
	while (i < fat.n_arch)
	{
		obj = data->content.ar.contents[i];
		write(STDOUT_FILENO, obj.name, ft_strlen(obj.name));
		write(STDOUT_FILENO, ":\n", 2);
		list_all_symbols(obj.bin);
		i++;
	}
}
*/
