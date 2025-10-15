# ft_ls
> An home-made version of the famous *ls* unix command.

> **VOIR COMMENTAIRE GET_ENTRIES()**

## Summary:
- [Usage](#usage)
- [How to test](#how-to-test)
- [Goals](#goals)
	- [General](#general)
	- [In details](#in-details)
- [Constraints](#constraints)
	- [Allowed functions](#allowed-functions)
- [Theory](#theory)
	- [Syntax](#syntax)
	- [Modularity](#modularity)
	- [Options](#options)
- [In practice](#in-practice)
	- [The parser](#1the-parser)
	- [The basics](#2the-basics-readdir-and-display-function)
	- [Recursive](#3recursive--r)
	- [List all](#4list-all--a)
	- [Sorting parameters](#5sorting-parameters--r-t)
	- [Long format](#6long-format--l)

## Usage:
	TODO

## How to test:
Run this command:
LSFLAG="" && DIR="" && ./ft_ls $LSFLAG $DIR; echo '========='; ls $LSFLAG $DIR

The two variables LSFLAG and DIR will be the arguments passed to ft_ls and ls:
- **LSFLAG** contains the options (ex:-at)
- **DIR**	contains the path we want to list
Each one can be empty. (Example: LSFLAG="-l" && DIR="" <=> "ls -l")
It will display first the output of *ft_ls*, then ,whatever happens, the output of *ls*.
**Carefull!** You cannot put multiple path in *DIR*. Your shell will interprete the variable has one unique argument.

## Goals:
### General:
- **Recreate** the *ls* Unix command in C.
- Learn **new functions** from operating system's API associated with data structures.
- Think about the code structure **before starting** due to certains flags (-R: recursive).

### In details:
Must recreate the behaviour of the *ls* command with and without the following options: -l, -R, -a, -r, -t.

The project must be written in **C**.

The overall display must be **as indentical** as possible to the original ls.

It is **not required** to manage the **columns display** when the *-l* option is not in the arguments.

No error or unexpected behaviour is to be observed (i.e no memory leaks, pendant fds, segfaults, etc.). If unsure handle as *ls* would.

## Constraints:

### Allowed functions:
The allowed functions are the followings:

## Theory:

### Syntax:
The *ls* command can be written with the following forms:
- ls
- ls -lRa
- ls -l -R -a
- ls dirname
- ls -l dirname
- ls -lRa dirname
- ls -l dirname -R
- ls -R dir1 dir2 -a

With all these possibilities we will need a **parser**. It will read the input and store the needed informations for the program to run.

### Modularity:
With the information given by the parser, for performance reasons, the code needs to adapt and not run code or system calls if not needed.<br>
We will then take special care to **separate** the code (in particular options) into clearly **defined modules**.

### Options:
Here is a detailed list of the options required:
- **-l**: use a long listing format. One line for each item with informations about rights, type, owner, group,  size, last modified and the filename. >OK<
- **-R**: list subdirectories recursively. >OK<
- **-a**: do not ignore entries starting with '.' . >OK<
- **-r**: reverser order while sorting. >OK<
- **-t**: sort by time, newest first. >OK<

As bonuses :
- **-i**: prints the inode (unique identifier) of the file. >OK<
- **-n**: like -l but prints the user and group ID instead of names. >OK<
- **-d**: list directory themself, not their content. >OK<
- **-U**: do not sort directory entries. >OK<
- **-f**: same as -aU >OK<

## In practice:

### 1/	The parser

```c
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

	// display parameters
	uint8_t		lgest_fname;
	uint32_t	nb_entries;
	uint16_t	nb_to_list;

	// env data
	uid_t	userid;
	char	*current_dir;
}				t_ftls;
```
The parser, will store the data read in the user input in the above structure.</br>
In the first five fields, the options are stored. As an example, if the argument "-t" is in **argv, the field ```bool time_sort;``` will turn to 1 to be read later.</br>
The field ```char	**to_list;``` stored the files specified by the user. If no input it stays NULL;

### 2/	The basics: readdir() and display function
In each case/options, a list of files (if present) is required.

<ins>Let's code the core of the *ls* command</ins>:
```c
bool	get_entries(t_ftls *data)
{
	struct dirent	*entry = NULL;
	t_file_list		*tmp_file = NULL;
	DIR				*dir = opendir(data->current_dir);
	struct stat		*tmp_stat = ft_calloc(1, sizeof(struct stat));
	if (dir == NULL || tmp_stat == NULL)
		return (err_get_entries(errno, dir, tmp_stat, NULL));

	while ((entry = readdir(dir)) != NULL)
	{
		tmp_file = ft_calloc(1, sizeof(t_file_list));
		if (tmp_file == NULL)
			return (err_get_entries(errno, dir, tmp_stat, data->raw_entries));
		ft_memmove(&(tmp_file->file.dirent), entry, sizeof(struct dirent));
		if (ft_strlen(tmp_file->file.dirent.d_name) > data->lgest_fname)
			data->lgest_fname = ft_strlen(tmp_file->file.dirent.d_name);
		if (data->long_format == 1 || data->time_sort == 1)
		{
			if (stat(data->current_dir, tmp_stat) == -1)
				return (err_get_entries(errno, dir, tmp_stat, data->raw_entries));
			tmp_file->file.stat = *tmp_stat;
		}
		ft_lstadd_back(&(data->raw_entries), tmp_file);
		++data->nb_entries;
	}
	leave_get_entries(dir, tmp_stat, entry);
	return (0);
}
```
This function **reads and stores** all needed information (in a linked list) from files in the path specified by *data->current_dir*.</br>
The function **struct dirent \*readdir(DIR \*)** reads a new occurence each time it is called. When nothing is left to read, it returns a NULL pointer.</br>
The *struct dirent* structure contains **basic data** about the occurence as the name or the type.</br>
<ins>Looking forward</ins>: That is **enough for the basic ls** but not for the *-l* and *-t* options. That is why **int stat(const char *, struct stat *)** is wrapped in a condition:
```c
if (data->long_format == 1 || data->time_sort == 1)
{
	if (stat(data->current_dir, tmp_stat) == -1)
		return (err_get_entries(errno, dir, tmp_stat, data->raw_entries));
	tmp_file->file.stat = *tmp_stat;
}
```
<ins>Let's print the data</ins>:
```c
void	display(t_file_list *entries, bool list_all)
{
	t_file_list	*runner = entries;

	while (runner != NULL)
	{
		if (list_all == 1 || runner->file.dirent.d_name[0] != '.')
			ft_printf("%s  ", runner->file.dirent.d_name);
		runner = runner->next;
	}
	ft_printf("\n");
}

void	ftls_display(t_ftls *tmp_data)
{
	if (tmp_data->recursive == 1 || tmp_data->to_list != NULL)
		ft_printf("%s:\n", tmp_data->current_dir);
	sort_entries(ALPHAB, tmp_data);
	display(tmp_data->raw_entries, tmp_data->list_all);
}
```
Line by line:
```c
if (tmp_data->recursive == 1 || tmp_data->to_list != NULL)
	ft_printf("%s:\n", tmp_data->current_dir);
```
if recursive mode OR file specified by user, print the directory currently being listed.
```c
sort_entries(ALPHAB, tmp_data);
```
The output of the basic *ls* is alphabetically sorted.
```c
display(tmp_data->raw_entries, tmp_data->list_all);
```
Simply run trough the linked list of the entries read by the *get_entries()* function and print the struct direct's *d_name* field.

> With the above code, we can run a simple **ls** command without argument nor options.

### 3/	Recursive (-R)
> The recursive part of *ls* needs to be done from the very beginning.

<ins>Let's code</ins>:
```c
bool	recursive(t_ftls *tmp_data, t_file_list *r_entries)
{
	char	*d_name = NULL;

	r_entries = tmp_data->raw_entries;
	while (r_entries != NULL)
	{
		d_name = r_entries->file.dirent.d_name;
		if (r_entries->file.dirent.d_type == DT_DIR)
		{
			if (ft_strncmp(d_name, ".", ft_strlen(d_name)) != 0
			&& ft_strncmp(d_name, "..", ft_strlen(d_name)) != 0)
			{
				if (d_name[0] == '.' && tmp_data->list_all == 0)
					goto _next;
				ft_printf("\n");
				if (ftls(tmp_data, d_name) == 1)
				{
					free(tmp_data->current_dir);
					free_entries(tmp_data->raw_entries);
					return (1);
				}
			}
		}
		_next:
		r_entries = r_entries->next;
	}
	return (0);
}

bool	ftls(t_ftls *data, char *dirname)
{
	t_file_list	*r_entries = NULL;
	t_ftls		tmp_data = {0};

	ft_memcpy(&tmp_data, data, sizeof(t_ftls));
	tmp_data.raw_entries = NULL;
	if ((tmp_data.current_dir = path_update_subdirtmp_data.current_dir, dirname)) == NULL)
		return (1);
	if (get_entries(&tmp_data) == 1)
	{
		free(tmp_data.current_dir);
		return (1);
	}
	ftls_display(&tmp_data);
	if (tmp_data.recursive == 1
		&& recursive(&tmp_data, r_entries) == 1)
			return (1);
	else
		ft_printf("\n);
	free(tmp_data.current_dir);
	free_entries(tmp_data.raw_entries);
	return (0);
}
```

The recursive part is called in the ```bool	ftls(t_ftls *, char *);``` function at the line:
```c
if (tmp_data.recursive == 1
	&& recursive(&tmp_data, r_entries) == 1)
		return (1);
```
The function ```bool	recursive(t_ftls *, t_file_list *);``` runs through all the entries read in the *data->current_dir*. When an entry is typed as directory, the function calls ```ftls()``` recursively.</br>
When all the entries of the new *data->current_dir* are listed, the previous one can continue listing.

> We now have a solid base to add the next options.


###	4/List all (-a)
Well, we simply **print the items begining with a '.' if the "data.list_all" parameter equals 1**.
In the function ```void display(t_file_list *entries, bool long format, bool list_all);```, while our runner pointer variable runs through the list of entries, we check the parameter "list_all".
```c
while (runner != NULL)
{
	if (list_all == 1 || runner->file.dirent.d_name[0] != '.')
	{
		if (long_format == 1)
			display_long_format(runner, long_format_data);
		else
			ft_printf("%s  ", runner->file.dirent.d_name);
	}
	runner = runner->next;
}
```
Same logic for the recursive listing in ```bool	recursive(t_ftls *, t_file_list *);``` line 15.
```c
if (d_name[0] == '.' && tmp_data->list_all == 0)
	goto _next;
```
###	5/	Sorting parameters (-r/-t)
By default, the "ls" command lists in alphabetical order.
When calling the C function ```struct dirent *readdir(DIR *__dirp);``` the entities we get are not sorted, we simlpy read the memory as it is.
So we will have to sort them by hand in our code. I simply implemented a bubble sort algorithm as long as we don't manage thousands of items.
```
void	sort_entries(uint8_t type, t_ftls *data)
{
	t_file_list	*rs[2] =  {data->raw_entries, data->raw_entries->next};	// runners trough list
	t_file		tmp = {0};		// swap
	int			tmp_res = 0;
	long		tm[2] = {0};	// time
	bool		cont = 0;		// continue ?

	while (check_sorted_entries(data->raw_entries, type) == 0)
	{
		if (rs[1] == NULL)
		{
			rs[0] = data->raw_entries;
			rs[1] = rs[0]->next;
		}
		if ((type & 1) == 1)
		{
			tm[0] = (rs[0]->file.stat.st_mtim.tv_sec * 1000000000) + rs[0]->file.stat.st_mtim.tv_nsec;
			tm[1] = (rs[1]->file.stat.st_mtim.tv_sec * 1000000000) + rs[1]->file.stat.st_mtim.tv_nsec;
		}
		cont = 0;
		switch (type)
		{
			case ALPHAB:
				tmp_res = filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name);
				cont = (tmp_res < 0 || (tmp_res == 0 && ft_strcmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) > 0)) & 1;
				break ;
			case TIME:
				cont = (tm[0] >= tm[1]) & 1;
				break ;
			case R_ALPHAB:
				tmp_res = filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name);
				cont = (tmp_res > 0 || (tmp_res == 0 && ft_strcmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) < 0)) & 1;
				fflush(stdout);
				break ;
			case R_TIME:
				cont = (tm[0] <= tm[1]) & 1;
				break ;
			default:
				break ;
		}
		if (cont == 1)
		{
			rs[0] = rs[0]->next;
			rs[1] = rs[0]->next;
		}
		else
		{
			tmp = rs[0]->file;
			rs[0]->file = rs[1]->file;
			rs[1]->file = tmp;
		}
	}
}
```
In this function, we are looping through the entries while they are not sorted.
While looping, we check if the **time sort parameter** (bit in this case) equals 1 to get the timestamp if necessary.
The **switch instruction**, conditionned by the sorting type, will then trigger the **right comparison** to run.
Following the result of the comparison, we **swap** the two entries **or pass** to the next ones.

###	6/	Long format (-l)
This option display more informations about each entity that is listed. (see more [here](#options)).</br>
Because we will need more information, we will use :
- ```struct passwd *getpwuid(uid_t uid);```
- ```struct group *getgrgid(gid_t gid);```
- ```int stat(const char *restrict path, struct stat *restrict statbuf);```

These function will give much more informations about an entity as the owners, the number of blocks allocated, etc.
With the functions above in mind, the code is pretty straight forward, as you only have to format the data for the display.
For more info you can check the function ```void	get_long_format_data(t_file_list *entries, uint8_t long_format_data[4], uint32_t *total_blocks, const bool list_all)```.

### 7/	List itself (-d)
This option seems useless when running a simple "ls -d". It simply prints you a dot '.' .
This function is made to get information about a specific directory without having a complete flooded output. Ex: 'ls -ld ~'.
I cannot put all the code modified but I can explain the idea.
Wihtout -d the argument you give to the program is passed to opendir(), then to readdir(). The ouputs of readdir() are the data you are printing.
With -d it is your arguments are the data you want to print. Because the -d print the folder itlself.
So instead of open the directory and loop to get all the outputs in it. We have to loop our arguments, opendir() it, then get the . file and print the data from it.
Carefull you have to change the '.' with the argument when you are listing actuals arguments.


### 8/	Other options (-)
