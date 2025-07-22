#include "../inc/ft_ls.h"

int8_t	set_env(t_ftls *data, char **env)
{
	(void)env;
	get_pwd(data, env);
	if (data->pwd == NULL)
		return (-1);
	get_uid(data);
	return (data->userid);
}

bool	get_entries(t_ftls *data)
{
	DIR				*dir = opendir(data->pwd);
	struct stat		*tmp_stat = NULL;
	struct dirent	*entry = NULL;
	t_file_list		*tmp_file = NULL;

	if (dir == NULL)
		print_err(errno);
	while ((entry = readdir(dir)) != NULL)
	{
		tmp_file = ft_calloc(1, sizeof(t_file_list));
		if (tmp_file == NULL)
		{
			print_err(errno);
			free(dir);
			return (1);
		}
		memmove(&(tmp_file->file.dirent), entry, sizeof(struct dirent));
		if (ft_strlen(tmp_file->file.dirent.d_name) > data->lgest_fname)
			data->lgest_fname = ft_strlen(tmp_file->file.dirent.d_name);
		if (data->long_format == 1 || data->time_sort == 1)
		{
			if (stat(data->pwd, tmp_stat) == -1)
			{
				print_err(errno);
				free(dir);
				return (1);
			}
			tmp_file->file.stat = *tmp_stat;
		}
		ft_lstadd_back(&(data->raw_entries), tmp_file);
		++data->nb_entries;
	}
	free(entry);
	free(dir);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_ftls	data = {0};
	
	parser(argc, argv, &data);

	if (set_env(&data, env) == -1)
	{
		write(2, "ft_ls: failed to collect env variables\n", 40);
		free_tab(data.to_list);
		exit(1);
	}
	printf("> Env OK <\n");
	get_entries(&data);
	printf("> Entries OK <\n");
	sort_entries(ALPHAB, &data);
	display(&data);
	
	free_t_ftls(&data);
	return (0);
}
