#ifndef FTLS_H
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
#include <limits.h>
#include <grp.h>

#include "../ft_libft/inc/libft.h"
#include "../ft_printf/inc/ft_printf.h"

#define SEC_IN_MONTH	2629746
#define SEC_IN_DAY		86400
#define SEC_IN_HOUR		3600
#define SEC_IN_MIN		60
#define RECURSIVE		1
#define LONG_FORMAT		2
#define LIST_ALL		4
#define REVERSED		8
#define TIME_SORT		16
#define ITSELF			32
#define INODES			64
#define IDS				128
#define DONT_SORT		256

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
	R_ALPHAB,
	R_TIME
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
	bool	itself;
	bool	inodes;
	bool	ids;
	bool	dont_sort;
	uint16_t	options;

	char		**to_list;
	t_file_list	*raw_entries;	// linked list
	// t_file		*entries;		// tab

	// display parameters
	uint8_t		lgest_fname;
	uint32_t	nb_entries;
	uint16_t	nb_to_list;
	
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
bool	check_sorted_entries(t_file_list *raw_entries, uint8_t type);
void	sort_entries(uint8_t type, t_ftls *data);
bool	check_sorted_tolist(char **to_list, bool reversed);
void	sort_tolist(t_ftls *data);

/* === display.c === */
void	display_rights(t_file_list *entry);
void	display_long_format(t_file_list *entry, uint8_t long_format_data[4], bool itself);
void	display(t_ftls *data, char *dirname);
// void	display(t_ftls *data);
void	print_usage(void);

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
bool	add_entry(const struct dirent *entry, char *buf_path, t_ftls *data);
void	leave_get_entries(DIR *dir, char *buf_path);
bool	err_get_entries(int	err, DIR *dir, char *buf_path);
bool	get_entries(t_ftls *data);
bool	get_itself(t_ftls *data);

/* === path.c === */
char	*format_path(char **oldpath);
char	*path_update_subdir(char *oldpath, char *to_add);
char	*path_update_file(char *oldpath, const char *to_add);


/* === ftls.c === */
// void	ftls_display(t_ftls *tmp_data);
void	ftls_display(t_ftls *tmp_data, char *dirname);
bool	ftls(t_ftls *data, char *dirname);

/* === main.c === */

#endif