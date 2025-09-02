#include "../inc/ft_ls.h"

int	main(int argc, char **argv)
{
	char		*root = NULL;
	t_ftls		data = {0};
	uint16_t	ctr = 0;
	
	parser(argc, argv, &data);
	printf("> Parser OK <\n");
		
	if (data.to_list == NULL)
	{
		root = ft_strdup(".");
		if (root == NULL)
			return (1);
	}
	if (set_env(&data) == -1)
	{
		free_tab(data.to_list);
		exit(1);
	}
	printf("> Env OK <\n");
	sort_tolist(&data);
	while (data.to_list[ctr] != NULL)
	{
		data.to_list[ctr] = format_path(&data.to_list[ctr]);
		if (ftls(&data, data.to_list[ctr]) == 1)
		{
			free_t_ftls(&data);
			exit(LS_ERR_FATAL);
		}
		++ctr;
	}
	free(root);	
	free_t_ftls(&data);
	return (0);
}
