/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2019/02/02 16:30:48 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		usage(int ret)
{
	ft_putstr_fd("Usage: nm [-su] <object file>\n", STDOUT_FILENO);
	exit(ret);
}

static int		parse_option(t_nm_otool *data, char *opt)
{
	if (*opt == '-')
	{
		opt++;
		while (*opt)
		{
			if (*opt == 'u')
				data->opts |= SHOW_UNDEF_ONLY;
			else if (*opt == 'a')
				data->opts |= SHOW_ALL;
			else if (*opt == '-')
				break ;
			else
				return (-1);
			opt++;
		}
		return (1);
	}
	return (0);
}

static void		nm_process(t_nm_otool *data)
{
	data->buffer = NULL;
	data->print = 1;
	if (load_file(data) != 0)
		write(STDERR_FILENO, "error\n", 6);
	else
		ft_nm(data, data->buffer);
	if (data->buffer != NULL)
		munmap(data->buffer, data->buffer_size);
}

static void		nm_all_files(t_nm_otool *data, char **argv)
{
	char		**tmp;

	tmp = argv;
	if (!*argv)
	{
		data->show_filename = 0;
		data->filename = "a.out";
		nm_process(data);
	}
	else
		while (*argv)
		{
			if (*(argv + 1) == NULL && tmp == argv)
				data->show_filename = 0;
			else
				data->show_filename = 1;
			data->filename = *argv;
			nm_process(data);
			argv++;
		}
}

int				main(int argc, char **argv)
{
	t_nm_otool	data;
	int			end_opts;

	ft_bzero(&data, sizeof(data));
	if (argc >= 1)
	{
		argv++;
		while (*argv)
		{
			end_opts = parse_option(&data, *argv);
			if (end_opts == -1)
				usage(EXIT_FAILURE);
			else if (end_opts == 0)
				break ;
			argv++;
		}
		nm_all_files(&data, argv);
	}
	else
		usage(EXIT_SUCCESS);
	return (0);
}
