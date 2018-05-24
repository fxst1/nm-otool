/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap_bytes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/17 20:20:45 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 13:08:40 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

unsigned int		ft_swap_bytes(unsigned int bytes)
{
	bytes = ((bytes << 8) & 0xFF00FF00) | ((bytes >> 8) & 0xFF00FF);
	return (bytes << 16) | (bytes >> 16);
}
