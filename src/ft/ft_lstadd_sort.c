#include "ft_nm_otool.h"

void 		ft_lstadd_sort(t_list **root, t_list *add, int (*cmp)(void*, void*))
{
	t_list	*tmp;
	t_list	*last;
	int		compare;

	tmp = *root;
	last = NULL;
	if (!tmp)
		*root = add;
	else
	{
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
}
