#include "../inc/ft_ls.h"

int	main(int argc, char **argv)
{
	char		*root = NULL;
	uint16_t	ctr = 0;
	t_ftls		data = {0};
	
	data.options = SORT;
	parser(argc, argv, &data);
	if (!data.to_list || (data.options & ITSELF))
	{
		root = ft_strdup("./");
		if (root == NULL)
			return (1);
	}
	if ((data.options & (SORT | TIME_SORT)))
		sort_tolist(&data);
	if (data.to_list && !(data.options & ITSELF))
	{
		while (data.to_list[ctr])
		{
			data.to_list[ctr] = format_path(&data.to_list[ctr]);
			if (ftls(&data, data.to_list[ctr]) == 1)
			{
				free_t_ftls(&data);
				exit(LS_ERR_FATAL);
			}
			++ctr;
		}
	}
	else
	{
		if (ftls(&data, root) == 1)
		{
			free(root);
			free_t_ftls(&data);
			exit(LS_ERR_FATAL);
		}
	}
	free(root);	
	free_t_ftls(&data);
	return (0);
}
