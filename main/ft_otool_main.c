/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_otool_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2018/04/30 14:44:27 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_nm_otool.h"

static void		usage(int ret)
{
	ft_putstr_fd("Usage: otool [-c] [-s segment section] <files ...>\n", STDOUT_FILENO);
	exit(ret);
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
				if (*((*argv) + 1) && *((*argv) + 2))
				{
					data->segment = *((*argv) + 1);
					data->section = *((*argv) + 2);
					(*argv) += 3;
					return (0);
				}
				else
					usage(EXIT_FAILURE);
			}
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

static void		otool_process(t_nm_otool *data)
{
	data->print = 1;
	if (data->segment == NULL && data->section == NULL)
	{
		data->segment = "__TEXT";
		data->section = "__text";
	}
	if (load_file(data) != 0)
		write(2, "otool: error\n", 10);
	else
		ft_otool(data, data->buffer);
	free(data->buffer);
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
			while (*argv)
			{
				data.filename = *argv;
				otool_process(&data);
				argv++;
			}
	}
	else
		usage(EXIT_SUCCESS);
	return (0);
}
