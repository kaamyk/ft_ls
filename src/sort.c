#include "../inc/ft_ls.h"


int16_t	nc_strcmp(char *s1, char *s2) // non-case_strcmp()
{
	while (*s1 || *s2)
	{
		if (ft_tolower(*s1) != ft_tolower(*s2))
			return (ft_tolower(*s1) - ft_tolower(*s2));
		s1++;
		s2++;
	}
	return (0);
}

int16_t	filename_cmp(char *fname1, char *fname2)
{
	if (fname1 && *fname1 == '.')
		++fname1;
	if (fname2 && *fname2 == '.')
		++fname2;
	if (fname1 == NULL && fname2 == NULL)
		return (0);
	else if (fname1 != NULL && fname2 == NULL)
		return (1);
	else if (fname1 == NULL && fname2 != NULL)
		return (-1);
	return (nc_strcmp(fname1, fname2));
}

bool	check_sorted_entries(t_file_list *raw_entries)
{
	t_file_list	*r = raw_entries;
	t_file_list *r1 = NULL;
	
	while (r->next != NULL)
	{
		r1 = r->next;
		if (filename_cmp(r->file.dirent.d_name, r1->file.dirent.d_name) > 0)
			return (0);
		r = r1;
	}
	return (1);
}

void	sort_entries(uint8_t type, t_ftls *data)
{
	(void)type;
	t_file_list	*r1 =  data->raw_entries;
	t_file_list	*r2 = NULL;
	t_file		tmp = {0};

	while (check_sorted_entries(data->raw_entries) == 0)
	{
		if (r2 == NULL)
		{
			r1 = data->raw_entries;
			r2 = r1->next;
		}
		if (filename_cmp(r1->file.dirent.d_name, r2->file.dirent.d_name) <= 0)
		{
			r1 = r1->next;
			r2 = r1->next;
			continue ;
		}
		tmp = r1->file;
		r1->file = r2->file;
		r2->file = tmp;
	}
}

bool	check_sorted_tolist(char **to_list)
{
	char	**r = to_list;
	char	**r1 = NULL;

	if (r != NULL && r + 1 != NULL)
		r1 = r + 1;
	else
		return (1);
	while (*r1 != NULL)
	{
		if (filename_cmp(*r, *r1) > 0)
			return (0);
		r = r1++;
	}
	return (1);
}

void	sort_tolist(t_ftls *data)
{
	char	**r = data->to_list;
	char	**r1 = NULL;
	char	*tmp = NULL;

	if (r != NULL && r + 1 != NULL)
		r1 = r + 1;
	else
		return ;
	while (check_sorted_tolist(data->to_list) == 0)
	{
		if (*r1 == NULL)
		{
			r = data->to_list;
			r1 = r + 1;
		}
		if (filename_cmp(*r, *r1) <= 0)
		{
			r = r1++;
			continue ;
		}
		tmp = *r;
		*r = *r1;
		*r1 = tmp;
	}
}
