#include "../inc/ft_ls.h"

/* === Tables === */

void	print_tab(char **tab)
{
	if (tab == NULL)
	{
		write(1, "Tab is empty\n", 14);
		return ;
	}
	while (*tab != NULL)
	{
		write(1, *tab, ft_strlen(*tab));
		write(1, "\n", 1);
		++tab;
	}
}

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

size_t	tab_len(char **tab)
{
	if (tab == NULL || *tab == NULL)
		return (0);

	uint32_t	l = 0;

	while (tab[++l] != NULL);
	return (l);
}

void	tab_alloc_exit(char **tab)
{
	free_tab(tab);
	exit(LS_ERR_FATAL);
}

char	**tab_append(char **tab, char *to_append)
{
	uint32_t i = 0;
	char	**joined = malloc(sizeof(char *) * (tab_len(tab) + 2));	// Allocate old tab + new entry + NULL ptr
	if (joined == NULL)
		return (NULL);

	if (tab != NULL)
	{
		while (tab[i] != NULL)
		{
			joined[i] = tab[i];
			i++;
		}
	}
	joined[i] = ft_strdup(to_append);
	if (joined[i] == NULL)
		tab_alloc_exit(joined);
	joined[++i] = NULL;
	free(tab);
	return (joined);
}