/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm_otool.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fjacquem <fjacquem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/24 13:02:28 by fjacquem          #+#    #+#             */
/*   Updated: 2019/02/05 21:02:30 by fjacquem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_OTOOL_H
# define FT_NM_OTOOL_H
# include "libft.h"
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdint.h>
# include <sys/stat.h>
# include <sys/mman.h>
# include <mach-o/loader.h>
# include <mach-o/arch.h>
# include <mach-o/fat.h>
# include <mach/machine.h>
# include <mach-o/nlist.h>
# define BASE_HEX "0123456789abcdef"
# define ERR_CORRUPT_FILE 0
# define SHOW_DEFAULT 0
# define SHOW_CHARACTERS 1
# define SHOW_ALL 1
# define SHOW_UNDEF_ONLY 2
# define MASK_64_BITS 0xFFFFFFFF
# define RANLIB_HEADER_SIZE 0x3C
# define RANLIB_HEADER_SIZE_OFFSET 0x30
# define RANLIB_LONG_NAME_OFFSET 0x44
# define ARCH_SIGN_LEN	0x8
# define ARCH_SIGN		"!<arch>\n"

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
	size_t				symbol_size;
	size_t				n_load_commands;
	size_t				size_load_commands;
	size_t				n_symbols;
	size_t				symtab_offset;
	size_t				strtab_size;
	size_t				strtab_offset;
	uint64_t			text_addr;
	uint32_t			text_offset;
	size_t				text_size;
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
	int					show_filename;
	int					swap_bytes;
	char				*filename;
	char				*objname;
	uint8_t				*buffer;
	size_t				buffer_size;
	int					opts;
	t_list				*symbols;
	char				*section;
	char				*segment;
	t_printerbuffered	pbuf;
}						t_nm_otool;

/*
**	Added functions
*/
void					*ft_print_memory(const void *addr, size_t size);
void					ft_putnbr_base_fd(int64_t nb, char *base, int fd);
void					ft_putnbr_base_offset_fd(int64_t nb, char *base,
							size_t offset, int fd);
void					ft_putnbr_base_buffer(int64_t nb, char *base,
							t_printerbuffered *buffer);
void					ft_putnbr_base_offset_buffer(int64_t nb, char *base,
							size_t offset, t_printerbuffered *buffer);
unsigned int			ft_swap_bytes(unsigned int bytes);
void					ft_lstiter2(t_list *lst, void (*fct)(void*, t_list*),
							void *data);
t_list					*ft_lstrev(t_list **root);
t_list					*ft_lstadvance(t_list *lst, size_t n);

/*
**	Symbols
*/
void					iter_symbols_default(void *nbits, t_list *lst);
int						compare_symbols(void *p1, void *p2);

/*
**	Load a binary file
*/
int						load_file(t_nm_otool *data);

/*
**	Nm
*/
int						ft_nm(t_nm_otool *data, uint8_t *buf);
int						ft_nm_macho64(t_nm_otool *data, uint8_t *buf);
int						ft_nm_macho32(t_nm_otool *data, uint8_t *buf);
int						ft_nm_ppc(t_nm_otool *data, uint8_t *buf);
int						ft_nm_fat(t_nm_otool *data, uint8_t *buf, int swap);
int						ft_nm_ar(t_nm_otool *data, uint8_t *buf);
void					ft_nm_clear(t_nm_otool *data);
void					ft_nm_print(t_nm_otool *data);
int						get_seg_sect_name_64(uint8_t *buf, size_t sect_index,
							t_symbol *s, size_t n_load_commands);
int						get_seg_sect_name_32(uint8_t *buf, size_t sect_index,
							t_symbol *s, size_t n_load_commands);
int						get_seg_sect_name_ppc(uint8_t *buf, size_t sect_index,
							t_symbol *s, size_t n_load_commands);
/*
**	Otool
*/
int						ft_otool(t_nm_otool *data, uint8_t *buf);
int						ft_otool_macho64(t_nm_otool *data, uint8_t *buf);
int						ft_otool_macho32(t_nm_otool *data, uint8_t *buf);
int						ft_otool_ppc(t_nm_otool *data, uint8_t *buf);
int						ft_otool_fat(t_nm_otool *data, uint8_t *buf, int swap);
int						ft_otool_ar(t_nm_otool *data, uint8_t *buf);
int						ft_otool_print_section(t_nm_otool *data,
							t_freader *reader, uint8_t *start, int is_ppc);

/*
**	Printers
*/
void					ft_printerinit(t_printerbuffered *pbuf, int fd,
							size_t allocsize, size_t flushsize);
void					ft_printerwrite(t_printerbuffered *pbuf,
							const void *buf, size_t buf_size);
void					ft_printerwrite_str(t_printerbuffered *pbuf,
							const char *buf);
void					ft_printerflush(t_printerbuffered *pbuf);
void					ft_printerclose(t_printerbuffered *pbuf);
void					ft_printerclose_flush(t_printerbuffered *pbuf);

/*
**	Achive helpers
*/
int						compare_ar_symbols(void *p1, void *p2);
uint32_t				get_size_name(uint8_t *buf);
int						read_object(t_nm_otool *data, uint8_t *buf,
							t_list **root);
int						append_unref_objects(t_nm_otool *data, uint8_t *buf,
							t_list **root);

/*
**	Errors
*/
int						display_error(t_nm_otool *data, const int errtype,
							const char *errmsg);
int						display_error_stdout(t_nm_otool *data,
							const int errtype,
							const char *errmsg);
int						corruption_error(t_nm_otool *data, const char *errmsg);
int						binary_strtab_corrupt(t_nm_otool *data, char *addr);
int						binary_is_corrupt(t_nm_otool *data, uint8_t *access,
							size_t len);

#endif
