#include "../inc/ft_ls.h"

bool	err_get_entries(int	err, DIR *dir, struct stat *tmp_stat, t_file_list *raw_entries, t_file_list *tmp_file)
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
	return (1);
}

void	leave_get_entries(DIR *dir, struct stat *tmp_stat, struct dirent *entry)
{
	closedir(dir);
	free(tmp_stat);
	free(entry);
}

bool	get_entries_init(const t_ftls *data, DIR **dir, struct stat **buf_stat, char **buf_path)
{
	*dir = opendir(data->current_dir);
	if (dir == NULL)
	{
		print_err(errno);
		return (1);
	}
	*buf_stat = ft_calloc(1, sizeof(struct stat));
	if (buf_stat == NULL)
	{
		print_err(errno);
		return (1);
	}
	*buf_path = ft_calloc(PATH_MAX, 1);
	if (buf_path == NULL)
	{
		print_err(errno);
		free(*buf_stat);
		return (1);
	}
	return (0);
}

bool	get_entries(t_ftls *data)
{
	struct dirent	*entry = NULL; // readdir()
	t_file_list		*new_file = NULL;
	DIR				*dir = NULL;
	struct stat		*buf_stat = NULL;
	char			*buf_path = NULL;
	printf("Current dir => %s\n", data->current_dir);

	if (get_entries_init(data, &dir, &buf_stat, &buf_path) == 1)
		return (1);
	ft_strcpy(buf_path, data->current_dir);
	while ((entry = readdir(dir)) != NULL)
	{
		printf("entry->d_name == %s\n", entry->d_name);
		new_file = ft_calloc(1, sizeof(t_file_list));
		if (new_file == NULL)
			return (err_get_entries(errno, dir, buf_stat, data->raw_entries, new_file));
		ft_memmove(&(new_file->file.dirent), entry, sizeof(struct dirent));
		if (ft_strlen(new_file->file.dirent.d_name) > data->lgest_fname)
			data->lgest_fname = ft_strlen(new_file->file.dirent.d_name);
		if (data->long_format == 1 || data->time_sort == 1)
		{
			buf_path =  path_update_file(buf_path, entry->d_name);
			// printf("entry->name == %s\n", entry->d_name);
			if (stat(buf_path, buf_stat) == -1)
				return (err_get_entries(errno, dir, buf_stat, data->raw_entries, new_file));
			new_file->file.stat = *buf_stat;
		}
		ft_lstadd_back(&(data->raw_entries), new_file);
		++data->nb_entries;
	}
	free(buf_path);
	leave_get_entries(dir, buf_stat, entry);
	return (0);
}