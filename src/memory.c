#include "../inc/ft_ls.h"

void	free_tab(char **tab)
{
	if (tab == NULL)
		return ;

	char **runner = tab;
	while (*runner != NULL)
	{
		free(*runner);
		++runner;
	}
	free(tab);
}

void	free_t_file(t_file *to_free)
{
	if (to_free == NULL)
		return ;
	if (to_free->fullpath != NULL)
		free(to_free->fullpath);
	free(to_free);
}

void	free_entries(t_file_list *to_free)
{
	if (to_free == NULL)
		return ;
	
	t_file_list	*runner = NULL;
	
	while (to_free != NULL)
	{
		runner = to_free->next;
		free_t_file(&(to_free->file));
		to_free = runner;
	}
}

void	free_t_ftls(t_ftls *to_free)
{
	free_tab(to_free->to_list);
	free_entries(to_free->raw_entries);
	if (to_free->current_dir != NULL)
		free(to_free->current_dir);
}
