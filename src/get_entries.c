#include "../inc/ft_ls.h"

void	leave_get_entries(DIR *dir, char *buf_path)
{
	if (dir != NULL)
		closedir(dir);
	if (buf_path != NULL)
		free(buf_path);
}

bool	err_get_entries(int	err, DIR *dir, char *buf_path)
{
	print_err(err);
	leave_get_entries(dir, buf_path);
	return (1);
}

bool	get_entries_init(const t_ftls *data, DIR **dir, struct stat **buf_stat, char **buf_path)
{
	*dir = opendir(data->current_dir);
	if (*dir == NULL)
	{
		print_err(errno);
		return (1);
	}
	*buf_stat = ft_calloc(1, sizeof(struct stat));
	if (*buf_stat == NULL)
	{
		print_err(errno);
		return (1);
	}
	*buf_path = ft_calloc(PATH_MAX, 1);
	if (*buf_path == NULL)
	{
		print_err(errno);
		free(*buf_stat);
		return (1);
	}
	return (0);
}

bool	add_entry(const struct dirent *entry, char **buf_path, t_ftls *data)
{
	t_file_list		*new_file = NULL;
	struct stat		buf_stat = {0};

	new_file = ft_calloc(1, sizeof(t_file_list));
	if (new_file == NULL)
	{
		print_err(errno);
		return (1);
	}
	ft_memmove(&(new_file->file.dirent), entry, sizeof(struct dirent));
	if (ft_strlen(new_file->file.dirent.d_name) > data->lgest_fname)
		data->lgest_fname = ft_strlen(new_file->file.dirent.d_name);
	if (data->options & (LONG_FORMAT | TIME_SORT | INODES))
	{
		*buf_path = path_update_file(*buf_path, entry->d_name);
		if (*buf_path == NULL || stat(*buf_path, &buf_stat) == -1)
		{
			free(new_file);
			return (1);
		}
		new_file->file.stat = buf_stat;
	}
	ft_lstadd_back(&(data->raw_entries), new_file);
	return (0);
}

bool	get_entries(t_ftls *data)
{
	struct dirent	*entry = NULL; // readdir()
	char			*buf_path = NULL;
	DIR				*dir = opendir(data->current_dir);

	if (dir == NULL)
	{
		write(STDERR_FILENO, "ft_ls: cannot open directory ", 29);
		write(STDERR_FILENO, data->current_dir, ft_strlen(data->current_dir));
		write(STDERR_FILENO, " ", 1);
		write(STDERR_FILENO, strerror(errno), ft_strlen(strerror(errno)));
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	buf_path = ft_calloc(PATH_MAX, 1);
	if (buf_path == NULL)
	{
		print_err(errno);
		return (1);
	}
	ft_strcpy(buf_path, data->current_dir);
	buf_path = format_path(&buf_path);
	while ((entry = readdir(dir)) != NULL)
	{
		if (add_entry(entry, &buf_path, data) == 1)
		{
			free(buf_path);
			closedir(dir);
			return (1);
		}
		++data->nb_entries;
	}
	leave_get_entries(dir, buf_path);
	return (0);
}

bool	get_itself(t_ftls *data)
{
	struct dirent	*entry = NULL; // readdir()
	char			*buf_path = NULL;
	char			**runner = data->to_list;
	DIR				*dir = NULL;

	buf_path = ft_calloc(PATH_MAX, 1);
	if (buf_path == NULL)
		return (err_get_entries(errno, dir, buf_path));
	ft_strcpy(buf_path, *runner);
	buf_path = format_path(&buf_path);
	if (buf_path == NULL)
		return (err_get_entries(errno, dir, buf_path));
	while (*runner != NULL)
	{
		dir = opendir(*runner);
		if (dir == NULL)
		{
			print_err(errno);
			continue ;
		}
		entry = readdir(dir);
		if (entry == NULL)
			return err_get_entries(errno, dir, buf_path);
		ft_strlcpy(buf_path, *runner, sizeof(*runner));
		buf_path = format_path(&buf_path);
		(void)add_entry(entry, &buf_path, data);
		closedir(dir);
		++runner;
	}
	free(buf_path);
	return (0);
}
