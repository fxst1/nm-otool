/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sections_char.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/20 15:54:59 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/20 16:01:49 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

char		sections_char(char **names)
{
	if (!ft_strncmp(names[1], "__text", 6))
		return ('T');
	else if (!ft_strncmp(names[0], "__DATA", 6) &&
			!ft_strncmp(names[1], "__bss", 5))
		return ('B');
	else if (!ft_strncmp(names[1], "__data", 6))
		return ('D');
	else
		return ('S');
}
