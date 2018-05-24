/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:08:24 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 13:25:09 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void	mysort(t_list **root, t_list *add, t_list *tmp,
					int (*cmp)(void*, void*))
{
	t_list	*last;
	int		compare;

	last = NULL;
	while (tmp)
	{
		compare = (*cmp)(add->content, tmp->content);
		if (compare < 0)
		{
			add->next = tmp;
			if (last)
				last->next = add;
			else
				*root = add;
			return ;
		}
		last = tmp;
		tmp = tmp->next;
	}
	if (last)
		last->next = add;
	else
		*root = add;
}

void		ft_lstadd_sort(t_list **root, t_list *add, int (*cmp)(void*, void*))
{
	t_list	*tmp;

	tmp = *root;
	if (!tmp)
		*root = add;
	else
		mysort(root, add, tmp, cmp);
}
