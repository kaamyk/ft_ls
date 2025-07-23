#include "../inc/ft_ls.h"

int	main(int argc, char **argv)
{
	t_ftls	data = {0};
	
	parser(argc, argv, &data);
	printf("> Parser OK <\n");
		
	if ((data.current_dir = ft_strdup(".")) == NULL || set_env(&data) == -1)
	{
		free_tab(data.to_list);
		exit(1);
	}
	printf("> Env OK <\n");
	if (ftls(&data, NULL) == 1)
	{
		free_t_ftls(&data);
		exit(LS_ERR_FATAL);
	}
	
	free_t_ftls(&data);
	return (0);
}
