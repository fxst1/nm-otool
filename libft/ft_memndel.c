/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memndel.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 00:54:19 by fjacquem          #+#    #+#             */
/*   Updated: 2016/03/30 00:54:21 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "implemt.h"

void	ft_memndel(void **addr, size_t n)
{
	while (n--)
		free(addr[n]);
	free(*addr);
	*addr = NULL;
}
