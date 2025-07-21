#include "../inc/ft_ls.h"

int8_t	set_env(t_ftls *data, char **env)
{
	(void)env;
	// get_pwd(data, env);
	data->pwd = NULL;
	if (data->pwd == NULL)
		return (-1);
	get_uid(data);
	return (data->userid);
}

void	get_entries(t_ftls *data)
{
	char			*err_mes = NULL;
	DIR				*dir = opendir(data->pwd);
	struct dirent	*entry = NULL;
	struct stat		*tmp_stat = NULL;
	t_file			*tmp_file = NULL;

	if (dir == NULL)
		get_entrie_err(errno);
	while ((entry = readdir(dir)) != NULL)
	{
		ft_memset(&tmp_file, 0, sizeof(tmp_file));
		tmp_file = malloc(sizeof(t_file));
		tmp_file->dirent = *entry;
		if (data->long_format == 1 || data->time_sort == 1)
		{
			if (stat(data->pwd, tmp_stat) == -1)
				get_entries_err(errno);
			tmp_file->stat = *tmp_stat;
		}
		ft_lstadd_back(data->lst_entry, tmp_file);
	}
}

int	main ( int argc, char **argv, char **env )
{
	t_ftls	data = {0};
	
	parser(argc, argv, &data);

	if (set_env(&data, env) == -1)
	{
		free_tab(data.to_list);
		exit(1);
	}

	list(&data);
	
	free_tab(data.to_list);
	return (0);
}
