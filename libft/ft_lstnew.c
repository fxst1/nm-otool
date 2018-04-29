/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/24 13:09:37 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/27 19:11:20 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void const *content, size_t content_size)
{
	t_list *lst;

	if ((lst = (t_list*)malloc(sizeof(t_list))) != NULL)
	{
		lst->next = NULL;
		lst->content = malloc(content_size);
		if (lst->content)
		{
			ft_memcpy(lst->content, content, content_size);
			lst->content_size = content_size;
		}
		else
		{
			lst->content = NULL;
			lst->content_size = 0;
		}
	}
	return (lst);
}
