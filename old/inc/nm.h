/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:58:08 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/13 12:47:42 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H
# include <binary.h>
# define SHOW_USER_ONLY 1
# define SHOW_SYSCALL_ONLY 2
# define SHOW_ALL 4

typedef struct		s_nm
{
	char			*filename;
	t_binary		bin;
	int				opts;
}					t_nm;

void				nm_list_all(t_nm *data);

#endif
