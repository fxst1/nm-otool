/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2019/02/02 16:28:22 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		usage(int ret)
{
	ft_putstr_fd("Usage: otool [-c] [-s segment section] <files ...>\n",
		STDOUT_FILENO);
	exit(ret);
}

static void		parse_option_s(t_nm_otool *data, char ***argv)
{
	if (*((*argv) + 1) && *((*argv) + 2))
	{
		data->segment = *((*argv) + 1);
		data->section = *((*argv) + 2);
		(*argv) += 3;
	}
	else
		usage(EXIT_FAILURE);
}

static int		parse_option(t_nm_otool *data, char *opt, char ***argv)
{
	if (*opt == '-')
	{
		opt++;
		while (*opt)
		{
			if (*opt == 'c')
				data->opts |= SHOW_CHARACTERS;
			else if (*opt == 's' && !*(opt + 1))
			{
				parse_option_s(data, argv);
				return (0);
			}
			else if (*opt == '-')
				break ;
			else if (*opt != 't')
				return (-1);
			opt++;
		}
		return (1);
	}
	return (0);
}

static void		otool_process(t_nm_otool *data, char **argv)
{
	while (*argv)
	{
		data->buffer = NULL;
		data->filename = *argv;
		data->print = 1;
		if (data->segment == NULL && data->section == NULL)
		{
			data->segment = "__TEXT";
			data->section = "__text";
		}
		if (load_file(data) != 0)
			write(2, "otool: error\n", 13);
		else
			ft_otool(data, data->buffer);
		if (data->buffer != NULL)
			munmap(data->buffer, data->buffer_size);
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
			end_opts = parse_option(&data, *argv, &argv);
			if (end_opts == -1)
				usage(EXIT_FAILURE);
			else if (end_opts == 0)
				break ;
			argv++;
		}
		if (!*argv)
			usage(EXIT_FAILURE);
		else
			otool_process(&data, argv);
	}
	else
		usage(EXIT_SUCCESS);
	return (0);
}
