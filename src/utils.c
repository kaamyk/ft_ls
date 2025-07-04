#include "../inc/ft_ls.h"

/* === Tables === */

void	print_tab(char **tab)
{
	while (*tab != NULL)
	{
		printf("%s\n", *tab);
	}
}

void	free_tab(char **tab)
{
	while (*tab != NULL)
	{
		free(*tab);
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

char	**tab_append(char **tab, char *to_append)
{
	char	**joined = malloc(tab_len(tab) + 2);	// Allocate old tab + new entry + NULL ptr
	uint32_t i = 0;

	while (tab[i++] != NULL)
	{
		joined[i] = tab[i];
	}
	joined[i] = ft_strcpy(tab[i], to_append);
	joined[i + 1] = NULL;
	free_tab(tab);
	return (joined);
}