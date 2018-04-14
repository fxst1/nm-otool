/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/14 16:01:01 by fjacquem          #+#    #+#             */
/*   Updated: 2018/04/14 16:01:45 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# define BASE_HEX "0123456789abcdef"

void				ft_memcpy(void *dst, void *src, size_t len);
void				ft_bzero(void *dst, size_t len);
size_t				ft_strlen(const char *s);
void				ft_putstr_fd(const char *s, const int fd);
void				ft_puthex_fd(long number, const int fd);
void				ft_putnbr_fd(long n, int fd);
void				ft_putnbr_base_fd(long nb, char *base, int fd);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_putnbr_base_offset_fd(long nb, char *base, size_t offset,
						int fd);

#endif
