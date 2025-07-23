#include "../inc/ft_ls.h"

char	*update_path(char *oldpath, char *to_add)
{
	char	*tmp = ft_strjoin(oldpath, to_add);
	if (tmp == NULL)
	{
		return (NULL);
	}

	free(oldpath);
	oldpath = ft_strjoin(tmp, "/");
	free(tmp);
	return (oldpath);
}

bool	ftls(t_ftls *data, char *dirname)
{
	printf("=== recursive (data->current_dir -> [%s], dirname -> [%s]) === \n", data->current_dir, dirname);
	t_file_list	*r_entries = data->raw_entries;
	
	if ((data->current_dir = update_path(data->current_dir, dirname)) == NULL)
	{
		print_err(errno);
		return (1);
	}
	printf("fullpath -> [%s]\n", data->current_dir);
	r_entries = NULL;
	ft_printf("%s:\n", data->current_dir);
	if (get_entries(data) == 1)
	{
		// free_entries(data->raw_entries);
		free(data->current_dir);
		return (1);
	}
	r_entries = data->raw_entries;
	display(data);
	if (data->recursive == 0)
		goto _end_ftls;
	while (r_entries != NULL)
	{
		printf("entry -> [%s]\n", r_entries->file.dirent.d_name);
		if (r_entries->file.dirent.d_type == DT_DIR)
		{ 
			if ((r_entries->file.dirent.d_name[0] == '.' && data->list_all == 1)
			|| (ft_strncmp(r_entries->file.dirent.d_name, ".", ft_strlen(r_entries->file.dirent.d_name)) != 0 
			&& ft_strncmp(r_entries->file.dirent.d_name, "..", ft_strlen(r_entries->file.dirent.d_name)) != 0))
			{
				if (ftls(data, r_entries->file.dirent.d_name) == 1)
				{
					free_entries(data->raw_entries);
					free(data->current_dir);
					return (1);
				}
			}
		}
		printf("data->next -> %p\n", r_entries->next);
		r_entries = r_entries->next;
	}
	_end_ftls:
	// free_entries(data->raw_entries);
	return (0);
}