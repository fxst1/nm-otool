/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstiter2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:08:53 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 13:08:53 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstiter2(t_list *lst, void (*fct)(void*, t_list*), void *data)
{
	while (lst)
	{
		(*fct)(data, lst);
		lst = lst->next;
	}
}
