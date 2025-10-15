#include "../inc/ft_ls.h"

void	ft_lstadd_front(t_file_list **lst, t_file_list *new)
{
	if (lst || new)
	{
		new->next = *lst;
		*lst = new;
	}
}

t_file_list	*ft_lstlast(t_file_list *lst)
{
	t_file_list	*ptr = NULL;

	if (!lst)
		return (NULL);
	ptr = lst;
	while (ptr)
	{
		if (!ptr->next)
			return (ptr);
		ptr = ptr->next;
	}
	return (ptr);
}

void	ft_lstadd_back(t_file_list **lst, t_file_list *new)
{
	t_file_list	*ptr = NULL;

	if (!lst || !new)
		return ;
	ptr = ft_lstlast(*lst);
	if (ptr)
		ptr->next = new;
	else
		*lst = new;
}

void	ft_lstfree(t_file_list *lst)
{
	t_file_list *next = NULL;
	
	while (lst)
	{
		next = lst->next;
		free(lst->file.fullpath);
		free(lst);
		lst = next;
	}
}