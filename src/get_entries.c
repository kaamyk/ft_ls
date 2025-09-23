#include "../inc/ft_ls.h"

bool	err_get_entries(int	err, DIR *dir, struct stat *tmp_stat, t_file_list *raw_entries, t_file_list *tmp_file, char *buf_path)
{
	print_err(err);
	if (dir != NULL)
		closedir(dir);
	if (tmp_stat != NULL)
		free(tmp_stat);
	if (raw_entries != NULL)
		free_entries(raw_entries);
	if (tmp_file != NULL)
		free(tmp_file);
	if (buf_path != NULL)
		free(buf_path);
	return (1);
}

void	leave_get_entries(DIR *dir, char *buf_path)
{
	closedir(dir);
	if (buf_path != NULL)
		free(buf_path);
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

bool	add_entry(const struct dirent *entry, char *buf_path, DIR *dir, t_ftls *data)
{
	t_file_list		*new_file = NULL;
	struct stat		buf_stat = {0};

	new_file = ft_calloc(1, sizeof(t_file_list));
	if (new_file == NULL)
		return (err_get_entries(errno, dir, &buf_stat, data->raw_entries, new_file, buf_path));
	ft_memmove(&(new_file->file.dirent), entry, sizeof(struct dirent));
	if (ft_strlen(new_file->file.dirent.d_name) > data->lgest_fname)
		data->lgest_fname = ft_strlen(new_file->file.dirent.d_name);
	if (data->long_format == 1 || data->time_sort == 1)
	{
		buf_path = path_update_file(buf_path, entry->d_name);
		if (stat(buf_path, &buf_stat) == -1)
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

	buf_path = ft_calloc(PATH_MAX, 1);
	if (buf_path == NULL)
	{
		print_err(errno);
		return (1);
	}
	ft_strcpy(buf_path, data->current_dir);
	format_path(&buf_path);
	while ((entry = readdir(dir)) != NULL)
	{
		if (add_entry(entry, buf_path, dir, data) == 1)
		{
			free(buf_path);
			closedir(dir);
		}
		++data->nb_entries;
	}
	free(buf_path);
	closedir(dir);
	return (0);
}

bool	get_itself(t_ftls *data)
{
	struct dirent	*entry = NULL; // readdir()
	char			*buf_path = NULL;
	DIR				*dir = opendir(data->current_dir);

	if (dir == NULL)
	{
		print_err(errno);
		return (1);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (ft_strncmp(entry->d_name, ".", 2) != 0)
			break ;
	}
	if (entry == NULL)
	return (1);
	return (add_entry(entry, buf_path, dir, data) == 1);
}
