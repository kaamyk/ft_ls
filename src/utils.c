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
	char	**joined = ft_calloc((tab_len(tab) + 2), sizeof(char *));	// Allocate old tab + new entry + NULL ptr
	if (joined == NULL)
	{
		print_err(errno);
		return (NULL);
	}

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