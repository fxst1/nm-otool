/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf_clear_64.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/14 19:21:38 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/14 20:33:42 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <binary.h>

void			elf_clear_64(t_elf64 elf)
{
	free(elf.sections);
}
