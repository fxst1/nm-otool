/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_is_corrupt.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:15:51 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 20:04:11 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

int			display_error(t_nm_otool *data, const int errtype,
				const char *errmsg)
{
	ft_putstr_fd(data->filename, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (errtype == ERR_CORRUPT_FILE)
		ft_putstr_fd("Corrupt file: ", STDERR_FILENO);
	ft_putstr_fd(errmsg, STDERR_FILENO);
	return (1);
}

int			display_error_stdout(t_nm_otool *data, const int errtype,
				const char *errmsg)
{
	ft_putstr_fd(data->filename, STDOUT_FILENO);
	ft_putstr_fd(": ", STDOUT_FILENO);
	if (errtype == ERR_CORRUPT_FILE)
		ft_putstr_fd("Corrupt file: ", STDOUT_FILENO);
	ft_putstr_fd(errmsg, STDOUT_FILENO);
	return (1);
}

int			corruption_error(t_nm_otool *data, const char *errmsg)
{
	display_error(data, ERR_CORRUPT_FILE, errmsg);
	return (1);
}

int			binary_strtab_corrupt(t_nm_otool *data, char *addr)
{
	if (binary_is_corrupt(data, (uint8_t*)addr, 1))
		return (1);
	while (*addr)
	{
		if (binary_is_corrupt(data, (uint8_t*)addr, 1) || !ft_isprint(*addr))
			return (1);
		addr++;
	}
	return (0);
}

int			binary_is_corrupt(t_nm_otool *data, uint8_t *access, size_t len)
{
	return ((uintptr_t)access + len - (uintptr_t)data->buffer >
				data->buffer_size);
}
