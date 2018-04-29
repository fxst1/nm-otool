#include "ft_nm_otool.h"

void	ft_putnbr_base_fd(long nb, char *base, int fd)
{
	int length;

	length = ft_strlen(base);
	if (length > 0)
	{
		if (nb < 0)
		{
			if (length == 10)
				write(fd, "-", 1);
			nb = -nb;
		}
		if (nb >= length)
		{
			ft_putnbr_base_fd(nb / length, base, fd);
			ft_putnbr_base_fd(nb % length, base, fd);
		}
		else
			write(fd, &base[nb], 1);
	}
}

static size_t	getoffset(long nb, char *base, size_t *n)
{
	int length;

	length = ft_strlen(base);
	if (length > 0)
	{
		if (nb < 0)
			nb = -nb;
		if (nb >= length)
		{
			getoffset(nb / length, base, n);
			getoffset(nb % length, base, n);
		}
		else
			(*n)++;
	}
	return (*n);
}

void			ft_putnbr_base_offset_fd(long nb, char *base, size_t offset,
											int fd)
{
	size_t		tmp;

	tmp = 0;
	offset = offset - getoffset(nb, base, &tmp);
	while (offset--)
		write(fd, "0", 1);
	ft_putnbr_base_fd(nb, base, fd);
}
