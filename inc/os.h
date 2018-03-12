/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   os.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fxst1 <fxst1@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/12 10:57:03 by fxst1             #+#    #+#             */
/*   Updated: 2018/03/12 10:57:03 by fxst1            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OS_H
# define OS_H
# if defined _WIN32 || defined _WIN64
#  define FTOS	"WINDOWS"
#  define FT_WIN
# elif defined __APPLE__
#  define FTOS	"APPLE"
#  define FT_APPLE
# elif defined __linux__
#  define FTOS	"LINUX"
#  define FT_LINUX
# else
#  error "Undefined explotation system ... abort"
# endif
#endif
