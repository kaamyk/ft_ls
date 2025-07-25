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

enum	ret_code
{
	LS_SUCCESS,
	LS_ERR_FILE,
	LS_ERR_FATAL
};

enum	sort_type
{
	ALPHAB,
	TIME,
};

typedef struct	s_file
{
	char			*fullpath;
	struct dirent	dirent;
	struct stat		stat;
}				t_file;

typedef struct	s_file_list
{
	t_file		file;			// Easier to sort
	// struct s_file_list	*prev;
	struct s_file_list	*next;
}				t_file_list;

typedef struct	s_ftls
{
	// options
	bool	recursive;
	bool	long_format;
	bool	list_all;
	bool	reversed;
	bool	time_sort;

	char		**to_list;
	t_file_list	*raw_entries;	// linked list
	// t_file		*entries;		// tab

	// display parameters
	uint8_t		lgest_fname;
	uint32_t	nb_entries;
	
	// env data
	uid_t	userid;
	char	*current_dir;
}				t_ftls;

/* === memory.c === */
void	free_tab(char **tab);
void	free_t_file(t_file *to_free);
void	free_entries(t_file_list *to_free);
void	free_t_ftls(t_ftls *to_free);

/* === sort.c === */
int16_t	nc_strcmp(char *s1, char *s2);
int16_t	filename_cmp(char *fname1, char *fname2);
bool	check_sorted_entries(t_file_list *raw_entries);
void	sort_entries(uint8_t type, t_ftls *data);
bool	check_sorted_tolist(char **to_list);
void	sort_tolist(t_ftls *data);

/* === display.c === */
void	display(t_file_list *entries, bool list_all);

/* === parser.c === */
void	parser(int argc, char **argv, t_ftls *opts);

/* === utils.c === */

/* === error.c === */
void	print_err(int err);

/* === env.c === */
void	get_uid(t_ftls *data);
int8_t	set_env(t_ftls *data);

/* === list.c === */
void		ft_lstadd_front(t_file_list **lst, t_file_list *new);
t_file_list	*ft_lstlast(t_file_list *lst);
void		ft_lstadd_back(t_file_list **lst, t_file_list *new);

//	Tables
void	print_tab(char **tab);
void	free_tab(char **tab);
size_t	tab_len(char **tab);
char	**tab_append(char **tab, char *to_append);

/* === get_entries.c === */
bool	err_get_entries(int	err, DIR *dir, struct stat *tmp_stat, t_file_list *tmp_file);
bool	get_entries(t_ftls *data);

/* === ftls.c === */
char	*format_path(char **oldpath);
char	*update_path(char *oldpath, char *to_add);
void	ftls_display(t_ftls *tmp_data, char	*to_list);
bool	ftls(t_ftls *data, char *dirname);

/* === main.c === */

#endif