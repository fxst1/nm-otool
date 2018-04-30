#include "ft_nm_otool.h"

void	ft_putnbr_base_fd(uint64_t nb, char *base, int fd)
{
	size_t	length;

	length = ft_strlen(base);
	if (length > 0)
	{
		if (nb >= length)
		{
			ft_putnbr_base_fd(nb / length, base, fd);
			ft_putnbr_base_fd(nb % length, base, fd);
		}
		else
			write(fd, &base[nb], 1);
	}
}

static size_t	getoffset(uint64_t nb, char *base, size_t *n)
{
	size_t		length;

	length = ft_strlen(base);
	if (length > 0)
	{
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

void			ft_putnbr_base_offset_fd(uint64_t nb, char *base, size_t offset,
											int fd)
{
	size_t		tmp;

	tmp = 0;
	getoffset(nb, base, &tmp);
	while (tmp < offset)
	{
		if (tmp == 10)
			write(fd, "1", 1);
		else
			write(fd, "0", 1);
		tmp++;
	}
	ft_putnbr_base_fd(nb, base, fd);
}
