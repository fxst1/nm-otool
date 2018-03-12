#include <stdlib.h>

void		ft_bzero(void *dst, size_t len)
{
	unsigned char	*a;
	size_t			i;

	a = (unsigned char*)dst;
	i = 0;
	while (i < len)
	{
		a[i] = 0;
		i++;
	}
}
