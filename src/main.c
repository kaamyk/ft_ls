#include "../inc/ft_ls.h"

void	list(t_options *data)
{
	
}

int	main ( int argc, char **argv )
{
	t_options	data = {0};
	
	parser(argc, argv, &data);

	list(&data);
	
	free_tab(data.to_list);
	return (0);
}
