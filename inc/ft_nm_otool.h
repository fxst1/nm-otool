#ifndef FT_NM_H
# define FT_NM_H
# include "libft.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>
# include <sys/stat.h>
# include <sys/mman.h>
# define ERR_CORRUPT_FILE 0
# define SHOW_ALL 1
# define SHOW_UNDEF_ONLY 2
# ifndef LC_SEGMENT_64
#  define CPU_TYPE_X86_64 0x7000001
#  define CPU_TYPE_I386 0x7000000
#  define MH_MAGIC_64 0xFEEDFACF
#  define MH_MAGIC 0xFEEDFACE
#  define FAT_CIGAM 0xBEBAFECA
#  define FAT_MAGIC 0xCAFEBABE
#  define LC_SEGMENT 0x1
#  define LC_SEGMENT_64 0x19
#  define LC_SYMTAB 0x2
#  define N_TYPE 0x0e
#  define N_UNDF 0x0
#  define N_EXT 0x1
#  define N_ABS 0x2
#  define N_INDR 0xa
#  define N_PBUD 0xc
#  define N_SECT 0xe
#  define N_STAB 0xe0
# endif
typedef struct stat		t_stat;

typedef struct			s_symbol
{
	char				character;
	char				*symbname;
	char				*segname;
	char				*sectname;
	uint64_t			value;
	uint32_t			type;
}						t_symbol;

typedef struct			s_freader
{
	size_t				n_load_commands;
	size_t				size_load_commands;
	size_t				n_symbols;
	size_t				symtab_offset;
	size_t				strtab_size;
	size_t				strtab_offset;
}						t_freader;

typedef struct			s_object
{
	uint32_t			symbol;
	uint32_t			offset;
}						t_object;

typedef struct			s_nm_otool
{
	int					nbits;
	int					print;
	int					swap_bytes;
	char				*filename;
	uint8_t				*buffer;
	size_t				buffer_size;
	int					opts;
	t_list				*objects;
	t_list				*symbols;
}						t_nm_otool;

void					ft_putnbr_base_fd(long nb, char *base, int fd);
void					ft_putnbr_base_offset_fd(long nb, char *base, size_t offset,
											int fd);
unsigned int			ft_swap_bytes(unsigned int bytes);
void					ft_lstiter2(t_list *lst, void (*fct)(void*, t_list*),
									void *data);
void 					ft_lstadd_sort(t_list **root, t_list *add,
										int (*cmp)(void*, void*));

void					iter_symbols_default(void *nbits, t_list *lst);
int						compare_symbols(void *p1, void *p2);

int						load_file(t_nm_otool *data);

int						ft_nm(t_nm_otool *data, uint8_t *buf);
int 					ft_nm_macho64(t_nm_otool *data, uint8_t *buf);
int 					ft_nm_macho32(t_nm_otool *data, uint8_t *buf);
int 					ft_nm_fat(t_nm_otool *data, uint8_t *buf, int swap);
int						ft_nm_ar(t_nm_otool *data, uint8_t *buf);
void					ft_nm_clear(t_nm_otool *data);
void					ft_nm_print(t_nm_otool *data);

int						display_error(t_nm_otool *data, const int errtype, const char *errmsg);
int						corruption_error(t_nm_otool *data, const char *errmsg);
int						binary_strtab_corrupt(t_nm_otool *data, char *addr);
int						binary_is_corrupt(t_nm_otool *data, uint8_t *access,
											size_t len);

#endif
