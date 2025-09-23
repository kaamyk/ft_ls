#include "../inc/ft_ls.h"

void	print_parser(t_ftls *data)
{
	ft_printf("===============\n");
	ft_printf("Recursive : %d\nLong format : %d\nList all : %d\nReverse : %d\nTime Sort : %d\n", \
			data->recursive, data->long_format, data->list_all, data->reversed, data->time_sort);
	ft_printf("===============\n");
}

void	leave_parser(char **to_list, const int err_code)
{
	free_tab(to_list);
	exit(err_code);
}

void	parser(const int argc, char **argv, t_ftls *data)
{
	if (argc == 1)
		return ;

	char	**runner = argv + 1;	// skip the program's name
	char	**to_list = NULL;
	
	while (*runner != NULL)
	{
		if ((*runner)[0] == '-') // item is an argument
		{
			if ((*runner)[1] == '-' && ft_strncmp(*runner, "--help", 6) == 0)
			{
				print_usage();
				leave_parser(to_list, LS_SUCCESS);
			}
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
					case 'd':
						data->itself = 1;
						break ;
					case 'i':
						data->inodes = 1;
						break ;
					case 'n':
					 	data->ids = 1;
						break ;
					case 'U':
					 	data->dont_sort = 1;
						break ;
					case 'f':
						data->itself = 1;
						data->list_all = 1;
						break ;
					default:
						ft_printf("ft_ls: invalid option \'%c\'\nTry \'ft_ls --help\' for more information.\n", **runner);
						leave_parser(to_list, LS_ERR_FATAL);
						break ;
				}
			}
		}
		else if (*runner != NULL)	// item is a filename/dirname
		{
			to_list = tab_append(to_list, *runner);
			if (to_list == NULL)
				exit(LS_ERR_FATAL);
			++data->nb_to_list;
		}
		++runner;
	}
	data->to_list = to_list;
}