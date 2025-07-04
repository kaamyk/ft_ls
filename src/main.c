#include "../inc/ft_ls.h"

int	main ( int argc, char **argv )
{
	t_options	data = {0};
	
	parser(argc, argv, &data);
	return (0);
}
