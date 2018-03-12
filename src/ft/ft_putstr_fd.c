#include <unistd.h>

size_t		ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (*s)
	{
		i++;
		s++;
	}
	return (i);
}

void		ft_putstr_fd(const char *s, const int fd)
{
	write(fd, s, ft_strlen(s));
}
