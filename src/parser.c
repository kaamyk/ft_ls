#include "../inc/ft_ls.h"

void	print_parser(t_options *data)
{
	ft_printf("===============\n");
	ft_printf("Recursive : %d\nLong format : %d\nList all : %d\nReverser : %d\nTime Sort : %d\n", \
			data->recursive, data->long_format, data->list_all, data->reversed, data->time_sort);
	ft_printf("===============\n");
}

void	parser(const int argc, char **argv, t_options *data)
{
	if (argc == 1)
		return ;

	char	**runner = argv + 1;	// skip the program's name
	char	**to_list = NULL;
	
	while (*runner != NULL)
	{
		if (*runner[0] == '-')
		{
			while (*(++*runner) != '\0')
			{
				switch (**runner)
				{
					case 'R':
						data->recursive = 1;
						break ;
					case 'l':
						data->long_format = 1;
						break ;
					case 'a':
						data->list_all = 1;
						break ;
					case 'r':
						data->reversed = 1;
						break ;
					case 't':
						data->time_sort = 1;
						break ;
					default:
					 	free_tab(to_list);
						printf("ft_ls: invalid option -- %c\nTry \'ft_ls --help\' for more information.\n", **runner);
						fflush(stdout);
						exit(LS_ERR_FATAL);
						break ;
				}
			}
		}
		else if (*runner != NULL)
		{
			to_list = tab_append(to_list, *runner);
		}
		++runner;
	}
	data->to_list = to_list;
	print_parser(data);
	ft_printf("to_list : \n");
	print_tab(data->to_list);
	ft_printf("=============");
	return ; 
}