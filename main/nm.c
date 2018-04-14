/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/14 15:17:33 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

static void		usage(int ret)
{
	ft_putstr_fd("Usage: nm [-su] <object file>\n", STDOUT_FILENO);
	exit(ret);
}

static int		parse_option(t_nm *data, char *opt)
{
	if (*opt == '-')
	{
		opt++;
		while (*opt)
		{
			if (*opt == 'u')
				data->opts |= SHOW_USER_ONLY;
			else if (*opt == 's')
				data->opts |= SHOW_SYSCALL_ONLY;
			else
				return (0);
			opt++;
		}
	}
	else
		data->filename = opt;
	return (1);
}

static void		nm_process(t_nm *data)
{
	if (binary_read(data->filename, &data->bin) != 0)
	{
		write(2, "nm: error\n", 13);
		binary_delete(&data->bin);
		exit(EXIT_FAILURE);
	}
	nm_list_all(data);
	binary_delete(&data->bin);
}

int				main(int argc, char **argv)
{
	t_nm		data;

	ft_bzero(&data, sizeof(data));
	if (argc >= 1)
	{
		argv++;
		while (*argv)
		{
			if (!parse_option(&data, *argv))
				usage(EXIT_FAILURE);
			argv++;
		}
		if (data.filename == NULL)
			data.filename = "a.out";
	}
	else
		usage(EXIT_SUCCESS);
	nm_process(&data);
	return (0);
}
