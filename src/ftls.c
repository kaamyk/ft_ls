#include "../inc/ft_ls.h"

void	ftls_display(t_ftls *tmp_data)
{
	uint8_t	sort_type = ALPHAB;

	if (!(tmp_data->options & ITSELF)
		&& ((tmp_data->options & RECURSIVE)
			|| (tab_len(tmp_data->to_list) > 1)))
	{
		ft_printf("%s:\n", tmp_data->current_dir);
	}
	if (tmp_data->options & (SORT | TIME_SORT))
	{
		if (tmp_data->options & TIME_SORT)
			sort_type |= 1;
		if (tmp_data->options & REVERSED)
			sort_type |= 2;
		sort_entries(sort_type, tmp_data);
	}
	display(tmp_data);
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
				if (d_name[0] == '.' && !(tmp_data->options & LIST_ALL))
					goto _next;
				write(STDOUT_FILENO, "\n", 1);
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
	if (((tmp_data.options & ITSELF) && get_itself(&tmp_data))
		|| (!(tmp_data.options & ITSELF) && get_entries(&tmp_data)))
	{
		free(tmp_data.current_dir);
		return (0);
	}
	ftls_display(&tmp_data);
	if (tmp_data.options & RECURSIVE)
	{
		if (recursive(&tmp_data, r_entries) == 1)
			return (1);
	}
	else if (tmp_data.options & ITSELF)
		write(STDOUT_FILENO, "\n", 1);
	free(tmp_data.current_dir);
	free_entries(tmp_data.raw_entries);
	return (0);
}