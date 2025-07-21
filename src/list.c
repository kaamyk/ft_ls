#include "../inc/ft_ls.h"

void	ft_lstadd_front(t_list **lst, t_list *new)
{
	if (lst || new)
	{
		new->next = *lst;
		*lst = new;
	}
}

t_file	*ft_lstlast(t_file *lst)
{
	t_file	*ptr;

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

void	ft_lstadd_back(t_file **lst, t_file *new)
{
	t_file	*ptr;

	if (!lst || !new)
		return ;
	ptr = ft_lstlast(*lst);
	if (ptr)
		ptr->next = new;
	else
		*lst = new;
}
