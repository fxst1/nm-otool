/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ar_get_name.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 16:42:24 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 16:47:28 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

char				*ar_get_name(t_ar_header *hobj)
{
	char			*ret;

	ret = (char*)hobj->name;
	if (!ft_strncmp(ret, "#1/", 3))
		return ((char*)((uint8_t*)hobj) + sizeof(t_ar_header));
	else
		return (ret);
}
