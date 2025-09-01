#include "../inc/ft_ls.h"

char	*format_path(char **oldpath)
{
	char	*last_sl = NULL;
	char	*tmp = NULL;

	last_sl = ft_strrchr(*oldpath, '/');
	if (last_sl != NULL)
	{
		if (*(last_sl - 1) == '/')
		{
			while (last_sl != *oldpath && *last_sl == '/')
				--last_sl;
		}
		tmp = ft_calloc(last_sl - *oldpath + 2, 1);
		ft_strlcpy(tmp, *oldpath, last_sl - *oldpath + 2);
		*oldpath = tmp;
	}
	return (*oldpath);
}


void	ftls_display(t_ftls *tmp_data)
{
	uint8_t	sort_type = ALPHAB;

	if ((tmp_data->recursive == 1 || tmp_data->to_list != NULL) && tmp_data->nb_to_list > 1)
		ft_printf("%s:\n", tmp_data->current_dir);
	if (tmp_data->time_sort == 1)
		sort_type |= 1;
	if (tmp_data->reversed == 1)
		sort_type |= 2;
	printf("sort_type = %d\n", sort_type);
	sort_entries(sort_type, tmp_data);
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
	if ((tmp_data.current_dir = path_update_subdir(tmp_data.current_dir, dirname)) == NULL)
		return (1);
	printf("Before get_entries() => tmp_data.current_dir -> %s\n", tmp_data.current_dir);
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