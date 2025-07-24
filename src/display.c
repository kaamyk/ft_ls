#include "../inc/ft_ls.h"

void	display(t_file_list *entries, bool list_all)
{
	t_file_list	*runner = entries;

	while (runner != NULL)
	{
		if (list_all == 1 || runner->file.dirent.d_name[0] != '.')
			ft_printf("%s  ", runner->file.dirent.d_name);
		runner = runner->next;
	}
	ft_printf("\n");
}
