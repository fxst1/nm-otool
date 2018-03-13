/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/13 12:47:53 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <nm.h>

static void		usage(void)
{
	ft_putstr_fd("Usage: nm [-su] <object file>\n", STDOUT_FILENO);
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
	nm_list_all(data);
	free(data->bin.buffer);
}

int				main(int argc, char **argv)
{
	t_nm		data;

	ft_bzero(&data, sizeof(data));
	if (argc >= 2)
	{
		argv++;
		while (*argv)
		{
			if (!parse_option(&data, *argv))
			{
				usage();
				return (1);
			}
			argv++;
		}
		if (data.filename == NULL)
		{
			usage();
			return (0);
		}
	}
	else
	{
		usage();
		return (0);
	}

	if (binary_read(data.filename, &data.bin) != 0)
	{
		write(2, "otool: error\n", 13);
		return (1);
	}
	nm_process(&data);
	return (0);
	(void)argc;
	(void)argv;
}
