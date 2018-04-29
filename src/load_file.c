#include "ft_nm_otool.h"

static int			error(const int fd, const char *err, const char *filename)
{
	write(2, "Cannot ", 7);
	ft_putstr_fd(err, 2);
	if (filename)
	{
		write(2, " ", 1);
		ft_putstr_fd(filename, 2);
	}
	write(2, "\n", 1);
	if (fd > 0)
		close(fd);
	return (-1);
}

int					load_file(t_nm_otool *data)
{
	int			fd;
	t_stat		st;

	fd = open(data->filename, O_RDONLY);
	if (fd == -1)
		return (error(fd, "open", data->filename));
	if (fstat(fd, &st) == -1)
		return (error(fd, "stat", data->filename));
	data->buffer_size = st.st_size;
	data->buffer = (uint8_t*)malloc(st.st_size);
	if (data->buffer == NULL || data->buffer == MAP_FAILED)
		return (error(fd, "allocate buffer", NULL));
	else if (read(fd, data->buffer, data->buffer_size) < 0)
		return (error(fd, "read", data->filename));
	close(fd);
	return (0);
}
