#ifndef OTOOL_H
# define OTOOL_H
# include <binary.h>
# define SHOW_HEADER 1
# define SHOW_TEXT 2
# define SHOW_SEGMENT 3

typedef struct		s_otool
{
	char			*filename;
	t_binary		bin;
	uint32_t		opts;
}					t_otool;

void	otool_header(t_otool *data);
void	otool_text(t_otool *data);

#endif
