#include "../inc/ft_ls.h"

void	parser(const int argc, char **argv, t_options *opts)
{
	if (argc == 1)
		return ;

	char	**runner = argv + 1;	// skip the program's name
	char	**to_list = NULL;
	
	while (*runner != NULL)
	{
		if (*runner[0] == '-')
		{
			while (**runner != '\0')
			{
				switch (**runner)
				{
					case 'R':
						opts->recursive = 1;
						break ;
					case 'l':
						opts->long_format = 1;
						break ;
					case 'a':
						opts->list_all = 1;
						break ;
					case 'r':
						opts->reversed = 1;
						break ;
					case 't':
						opts->time_sort = 1;
						break ;
					default:
						break ;
				}
			}
		}
		else
		{
			tab_append(to_list, *runner);
		}
		++(*runner);
	}
	print_tab(to_list);
	return; 
}