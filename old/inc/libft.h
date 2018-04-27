#ifndef LIBFT_H
# define LIBFT_H
# include <unistd.h>
# define BASE_HEX "0123456789abcdef"
void				ft_memcpy(void *dst, void *src, size_t len);
void				ft_bzero(void *dst, size_t len);
size_t				ft_strlen(const char *s);
void				ft_putstr_fd(const char *s, const int fd);
void				ft_puthex_fd(unsigned int number, const int fd);
void				ft_putnbr_fd(int n, int fd);
void				ft_putnbr_base_fd(int nb, char *base, int fd);
int					ft_strncmp(const char *s1, const char *s2, size_t n);
int					ft_strcmp(const char *s1, const char *s2);
void				ft_putnbr_base_offset_fd(int nb, char *base, size_t offset, int fd);

#endif
