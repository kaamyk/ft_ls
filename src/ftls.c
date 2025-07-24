#include "../inc/ft_ls.h"

char	*update_path(char *oldpath, char *to_add)
{
	char	*tmp = NULL;
	if (to_add != NULL)
		tmp = ft_strjoin(oldpath, "/");
	else
		tmp = ft_strdup(oldpath);
	if (tmp == NULL)
		return (NULL);
	oldpath = ft_strjoin(tmp, to_add);
	free(tmp);
	return (oldpath);
}

void	ftls_display(t_ftls *tmp_data)
{
	if (tmp_data->recursive == 1)
		ft_printf("%s:\n", tmp_data->current_dir);
	sort_entries(ALPHAB, tmp_data);
	display(tmp_data->raw_entries, tmp_data->list_all);
}

bool	ftls(t_ftls *data, char *dirname)
{
	t_file_list	*r_entries = NULL;
	char		*d_name = NULL;
	t_ftls		tmp_data = {0};

	ft_memcpy(&tmp_data, data, sizeof(t_ftls));
	tmp_data.raw_entries = NULL;
	printf("to_list -> ");
	print_tab(tmp_data.to_list);
	
	if ((tmp_data.current_dir = update_path(tmp_data.current_dir, dirname)) == NULL)
	{
		print_err(errno);
		return (1);
	}
	if (get_entries(&tmp_data) == 1)
	{
		free(tmp_data.current_dir);
		return (1);
	}
	ftls_display(&tmp_data);
	if (tmp_data.recursive == 0)
		goto _end_ftls;
	r_entries = tmp_data.raw_entries;
	while (r_entries != NULL)
	{
		d_name = r_entries->file.dirent.d_name;
		if (r_entries->file.dirent.d_type == DT_DIR)
		{ 
			if (ft_strncmp(d_name, ".", ft_strlen(d_name)) != 0 
			&& ft_strncmp(d_name, "..", ft_strlen(d_name)) != 0)
			{
				if (d_name[0] == '.' && data->list_all == 0)
					goto _next;
				ft_printf("\n");
				if (ftls(&tmp_data, d_name) == 1)
				{
					free(tmp_data.current_dir);
					free_entries(tmp_data.raw_entries);
					return (1);
				}
			}
		}
		_next:
		r_entries = r_entries->next;
	}
	_end_ftls:
	free(tmp_data.current_dir);
	free_entries(tmp_data.raw_entries);
	return (0);
}