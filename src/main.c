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


int	main(int argc, char **argv, char **env)
{
	t_ftls	data = {0};
	
	parser(argc, argv, &data);
	printf("> Parser OK <\n");

	if (set_env(&data, env) == -1)
	{
		free_tab(data.to_list);
		exit(1);
	}
	printf("> Env OK <\n");
	if (data.recursive == 1)
		recursive(&data);
	else if (get_entries(&data) == 1)
	{
		free_t_ftls(&data);
		exit(LS_ERR_FATAL);
	}
	printf("> Entries OK <\n");
	sort_entries(ALPHAB, &data);
	display(&data);
	
	free_t_ftls(&data);
	return (0);
}
