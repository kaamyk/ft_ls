#include "../inc/ft_ls.h"

char	*update_path(char *oldpath, char *to_add)
{
	// printf("update_path(%s, %s)\n", oldpath, to_add);
	char	*tmp = NULL;
	if (to_add != NULL)
	{
		tmp = ft_strjoin(oldpath, "/");
		if (tmp == NULL)
		{
			print_err(errno);
			return (NULL);
		}
		oldpath = ft_strjoin(tmp, to_add);
	}
	else
	{
		tmp = oldpath;
		oldpath = ft_strdup(tmp);
	}
	if (oldpath == NULL)
		print_err(errno);
	free(tmp);
	return (oldpath);
}

void	ftls_display(t_ftls *tmp_data)
{
	if (tmp_data->recursive == 1 || tmp_data->to_list != NULL)
		ft_printf("%s:\n", tmp_data->current_dir);
	sort_entries(ALPHAB, tmp_data);
	display(tmp_data->raw_entries, tmp_data->list_all);
}

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
	if ((tmp_data.current_dir = update_path(tmp_data.current_dir, dirname)) == NULL)
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
		ft_printf("\n");
	free(tmp_data.current_dir);
	free_entries(tmp_data.raw_entries);
	return (0);
}