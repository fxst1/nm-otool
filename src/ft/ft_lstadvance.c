/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadvance.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 18:05:19 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 18:08:55 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list		*ft_lstadvance(t_list *lst, size_t n)
{
	size_t		index;

	index = 0;
	while (lst && index < n)
	{
		lst = lst->next;
		index++;
	}
	return (lst);
}
