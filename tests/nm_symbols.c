/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm_symbols.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/14 17:04:29 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/14 17:05:45 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>

extern int		g_errno;

static int		g_mystatic = 0;
int				g_global = 0;

static void		fct(void)
{
	static int		tmp;

	(void)tmp;
}

int				main(int argc, char **argv)
{
	static int		tmp = 0;

	g_global = 0;
	g_mystatic = 0;
	errno = 0;
	return (0);
	(void)argc;
	(void)argv;
	(void)tmp;
}
