/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstrev.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 17:46:30 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 17:57:16 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void		ft_lstrev_rec(t_list *item, t_list **start)
{
	t_list		*next;

	if (item)
	{
		next = item->next;
		item->next = NULL;
		ft_lstadd(start, item);
		ft_lstrev_rec(next, start);
	}
}

t_list			*ft_lstrev(t_list **root)
{
	t_list	*ret;

	if (!root || !*root)
		return (NULL);
	ret = NULL;
	ft_lstrev_rec(*root, &ret);
	*root = ret;
	return (ret);
}
