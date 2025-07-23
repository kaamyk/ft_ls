#include "../inc/ft_ls.h"

void	display(t_ftls *data)
{
	t_file_list	*runner = data->raw_entries;

	while (runner != NULL)
	{
		if (data->list_all == 1 || runner->file.dirent.d_name[0] != '.')
			ft_printf("%s  ", runner->file.dirent.d_name);
		runner = runner->next;
	}
	ft_printf("\n");
}
