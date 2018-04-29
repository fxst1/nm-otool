#include "libft.h"

void	ft_lstiter2(t_list *lst, void (*fct)(void*, t_list*), void *data)
{
	while (lst)
	{
		(*fct)(data, lst);
		lst = lst->next;
	}
}
