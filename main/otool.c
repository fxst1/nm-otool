/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:16 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 10:57:17 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <otool.h>

static void		usage(void)
{
	ft_putstr_fd("Usage: otool [-ht] <object file>\n", STDOUT_FILENO);
}

static int		parse_option(t_otool *data, char *opt)
{
	if (*opt == '-')
	{
		opt++;
		while (*opt)
		{
			if (*opt == 'h')
				data->opts |= SHOW_HEADER;
			else if (*opt == 't')
				data->opts |= SHOW_TEXT;
			else
				return (0);
			opt++;
		}
	}
	else
		data->filename = opt;
	return (1);
}

static void		otool_process(t_otool *data)
{
	if (data->opts & SHOW_HEADER)
		otool_header(data);
	if (data->opts & SHOW_TEXT || data->opts == 0)
		otool_text(data);
}

int				main(int argc, char **argv)
{
	t_otool		data;

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
	otool_process(&data);
	return (0);
	(void)argc;
	(void)argv;
}
