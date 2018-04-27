#include <errno.h>
extern int		errno;

static int		g_mystatic = 0;
int				g_global = 0;

static void		fct(void)
{
	static int		tmp;

	(void)tmp;
}
int				main(int argc, char **argv)
{
	static int		tmp = 0;

	g_global = 0;
	g_mystatic = 0;

	return (0);
	(void)argc;
	(void)argv;
	(void)tmp;
}
