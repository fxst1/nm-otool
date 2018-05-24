/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_fat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:28:52 by fjacquem          #+#    #+#             */
/*   Updated: 2018/05/24 13:29:22 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int				ft_nm_fat(t_nm_otool *data, uint8_t *buf, int swap)
{
	uint8_t		*tmp;
	uint32_t	i;
	uint32_t	n_arch;
	uint32_t	offset;
	uint32_t	archs[1024];

	if (binary_is_corrupt(data, buf, 8))
		return (corruption_error(data, "Fat Header\n"));
	i = 0;
	n_arch = swap ? ft_swap_bytes(*((uint32_t*)buf + 1)) :
						*((uint32_t*)buf + 1);
	tmp = buf + 8;
	while (i < n_arch && i < 1024)
	{
		if (binary_is_corrupt(data, buf, 20))
			return (corruption_error(data, "Fat content\n"));
		archs[i] = *(uint32_t*)tmp;
		offset = swap ? ft_swap_bytes(*((uint32_t*)tmp + 2)) :
							*((uint32_t*)tmp + 2);
		if (archs[i] == CPU_TYPE_X86_64)
			return (ft_nm(data, buf + offset));
		tmp += 0x14;
		i++;
	}
	return (0);
}
