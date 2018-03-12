#include <stdlib.h>

void		ft_memcpy(void *dst, void *src, size_t len)
{
	unsigned char	*a;
	unsigned char	*b;
	size_t			i;

	a = (unsigned char*)dst;
	b = (unsigned char*)src;
	i = 0;
	while (i < len)
	{
		a[i] = b[i];
		i++;
	}
}
