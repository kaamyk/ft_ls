#include "../inc/ft_ls.h"

bool	err_get_entries(int	err, DIR *dir, struct stat *tmp_stat, t_file_list *tmp_file)
{
	print_err(err);
	if (dir != NULL)
		closedir(dir);
	if (tmp_stat != NULL)
		free(tmp_stat);
	if (tmp_file != NULL)
		free_entries(tmp_file);
	return (1);
}

void	leave_get_entries(DIR *dir, struct stat *tmp_stat, struct dirent *entry)
{
	closedir(dir);
	free(tmp_stat);
	free(entry);
}

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
		memmove(&(tmp_file->file.dirent), entry, sizeof(struct dirent));
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