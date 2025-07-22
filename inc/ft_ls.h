#ifndef	FTLS_H
#define FTLS_H

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <time.h>
#include <error.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <error.h>

#include "../ft_libft/inc/libft.h"
#include "../ft_printf/inc/ft_printf.h"

enum ret_code
{
	LS_SUCCESS,
	LS_ERR_FILE,
	LS_ERR_FATAL
};

typedef struct	s_file
{
	char			*fullpath;
	struct dirent	dirent;
	struct stat		stat;
	struct s_file	*prev;
	struct s_file	*next;
}				t_file;

typedef struct	s_ftls
{
	// options
	bool	recursive;
	bool	long_format;
	bool	list_all;
	bool	reversed;
	bool	time_sort;

	char	**to_list;
	t_file	*lst_entry;

	// display parameters
	uint8_t		lgest_fname;
	uint32_t	nb_entries;
	

	// env data
	uid_t	userid;
	char	*pwd;
	char	*current_dir;
}				t_ftls;

/* === memory.c === */
void	free_tab(char **tab);
void	free_t_file(t_file *to_free);
void	free_entries(t_file *to_free);
void	free_t_ftls(t_ftls *to_free);


/* === parser.c === */
void	parser(int argc, char **argv, t_ftls *opts);

/* === utils.c === */

/* === error.c === */
void	print_err(int err);

/* === env.c === */
void	get_pwd(t_ftls *data, char **env);
void	get_uid(t_ftls *data);

/* === list.c === */
void	ft_lstadd_front(t_list **lst, t_list *new);
t_file	*ft_lstlast(t_file *lst);
void	ft_lstadd_back(t_file **lst, t_file *new);

//	Tables
void	print_tab(char **tab);
void	free_tab(char **tab);
size_t	tab_len(char **tab);
char	**tab_append(char **tab, char *to_append);

#endif