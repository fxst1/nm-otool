/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsort.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/05 15:11:24 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 18:09:19 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		create_workers(t_mergesort *dat, size_t l, size_t m,
				size_t sizes[2])
{
	size_t			i;
	t_list			*t;

	i = 0;
	dat->left = NULL;
	dat->right = NULL;
	t = ft_lstadvance(dat->src, l);
	while (i < sizes[0])
	{
		ft_lstadd(&dat->left, ft_lstnew_noalloc(t->content, t->content_size));
		i++;
		t = t->next;
	}
	i = 0;
	t = ft_lstadvance(dat->src, m + 1);
	while (i < sizes[1])
	{
		ft_lstadd(&dat->right, ft_lstnew_noalloc(t->content, t->content_size));
		i++;
		t = t->next;
	}
	ft_lstrev(&dat->left);
	ft_lstrev(&dat->right);
	dat->src_advance = ft_lstadvance(dat->src, l);
}

static void		ft_merge_rest(size_t i, size_t size, t_list **src, t_list *w)
{
	t_list		*tmp;

	tmp = *src;
	while (i < size)
	{
		tmp->content = w->content;
		tmp = tmp->next;
		w = w->next;
		i++;
	}
	*src = tmp;
}

static void		ft_merge(t_mergesort dat, size_t sizes[2], t_list *wl,
				t_list *wr)
{
	size_t		i;
	size_t		j;
	t_list		*tmp;

	i = 0;
	j = 0;
	tmp = dat.src_advance;
	while (i < sizes[0] && j < sizes[1])
	{
		if ((*dat.cmp)(wl->content, wr->content) < 0)
		{
			tmp->content = wl->content;
			wl = wl->next;
			i++;
		}
		else
		{
			tmp->content = wr->content;
			wr = wr->next;
			j++;
		}
		tmp = tmp->next;
	}
	ft_merge_rest(i, sizes[0], &tmp, wl);
	ft_merge_rest(j, sizes[1], &tmp, wr);
}

static void		ft_mergesort(t_mergesort dat, size_t l, size_t r)
{
	size_t		middle;
	size_t		sizes[2];

	if (l < r)
	{
		middle = l + (r - l) / 2;
		ft_mergesort(dat, l, middle);
		ft_mergesort(dat, middle + 1, r);
		sizes[0] = middle - l + 1;
		sizes[1] = r - middle;
		create_workers(&dat, l, middle, sizes);
		ft_merge(dat, sizes, dat.left, dat.right);
		ft_lstdel(&dat.right, NULL);
		ft_lstdel(&dat.left, NULL);
	}
}

t_list			*ft_lstsort(t_list **lst, int (*cmp)(void*, void*))
{
	t_mergesort	dat;
	t_list		*tmp;
	size_t		n;

	if (lst && *lst && cmp)
	{
		tmp = *lst;
		dat.cmp = cmp;
		dat.src = tmp;
		n = 0;
		while (tmp)
		{
			n++;
			tmp = tmp->next;
		}
		ft_mergesort(dat, 0, n - 1);
		return (dat.src);
	}
	return (NULL);
}
