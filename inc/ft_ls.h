#ifndef	FTLS_H
#define FTLS_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <sys/xattr.h>
#include <time.h>
#include <stdlib.h>
#include <error.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../ft_libft/inc/libft.h"
#include "../ft_printf/inc/ft_printf.h"

enum ret_code
{
	LS_SUCCESS,
	LS_ERR_FILE,
	LS_ERR_FATAL
};

typedef struct s_options
{
	// options
	bool	recursive;
	bool	long_format;
	bool	list_all;
	bool	reversed;
	bool	time_sort;

	char	**to_list;
}				t_options;

/* === parser.c === */
void	parser(int argc, char **argv, t_options *opts);

/* === utils.c === */

//	Tables
void	print_tab(char **tab);
void	free_tab(char **tab);
size_t	tab_len(char **tab);
char	**tab_append(char **tab, char *to_append);

#endif