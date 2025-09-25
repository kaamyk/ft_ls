#include "../inc/ft_ls.h"

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
						data->options |= RECURSIVE;
						break ;
					case 'l':
						data->options |= LONG_FORMAT;
						break ;
					case 'a':
						data->options |= LIST_ALL;
						break ;
					case 'r':
						data->options |= REVERSED;
						break ;
					case 't':
						data->options |= TIME_SORT;
						break ;
					case 'd':
						data->options |= ITSELF;
						break ;
					case 'i':
						data->options |= INODES;
						break ;
					case 'n':
						data->options |= IDS;
						break ;
					case 'U':
						data->options ^= SORT;
						break ;
					case 'f':
						data->options |= LIST_ALL;
						data->options ^= SORT;
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
	if (to_list == NULL)
		to_list = tab_append(to_list, ".");
	data->to_list = to_list;
}