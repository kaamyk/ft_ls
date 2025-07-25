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

bool	check_sorted_entries(t_file_list *raw_entries, uint8_t type)
{
	t_file_list	*rs[2] =  {raw_entries, NULL};
	time_t		tm[2] = {0};
	
	while (rs[0]->next != NULL)
	{
		rs[1] = rs[0]->next;
		if ((type & 1) == 1)
		{
			printf("type time\n");
			tm[0] = (rs[0]->file.stat.st_mtim.tv_sec * 1000000000) + rs[0]->file.stat.st_mtim.tv_nsec;
			tm[1] = (rs[1]->file.stat.st_mtim.tv_sec * 1000000000) + rs[1]->file.stat.st_mtim.tv_nsec;
			printf("tm[0] == %ld | tm[1] = %ld\n", tm[0], tm[1]);
		}
		switch (type)
		{
			case ALPHAB:
				if (filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) > 0)
					return (0);
				break ;
			case TIME:
				if (tm[0] > tm[1])
					return (0);
				break ;
			case R_ALPHAB:
				if (filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) < 0)
					return (0);
				break ;
			case R_TIME:
				if (tm[0] < tm[1])
					return (0);
				break ;
			default:
				break ;
		}
		rs[0] = rs[1];
	}
	return (1);
}

void	sort_entries(uint8_t type, t_ftls *data)
{
	(void)type;
	t_file_list	*rs[2] =  {data->raw_entries, data->raw_entries->next};
	t_file		tmp = {0};
	long		tm[2] = {0};
	bool		cont = 0;

	printf("bf while type == %d\n", type);
	while (check_sorted_entries(data->raw_entries, type) == 0)
	{
		printf("type == %d | %d\n", type, (type & 1));
		if (rs[1] == NULL)
		{
			rs[0] = data->raw_entries;
			rs[1] = rs[0]->next;
		}
		if ((type & 1) == 1)
		{
			tm[0] = (rs[0]->file.stat.st_mtim.tv_sec * 1000000000) + rs[0]->file.stat.st_mtim.tv_nsec;
			tm[1] = (rs[1]->file.stat.st_mtim.tv_sec * 1000000000) + rs[1]->file.stat.st_mtim.tv_nsec;
			printf("type time\n");
			printf("tm[0] == %ld | tm[1] = %ld\n", tm[0], tm[1]);
			fflush(stdout);
		}
		cont = 0;
		switch (type)
		{
			case ALPHAB:
				cont = (filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) <= 0) & 1;
				break ;
			case TIME:
				cont = (tm[0] <= tm[1]) & 1;
				break ;
			case R_ALPHAB:
				cont = (filename_cmp(rs[0]->file.dirent.d_name, rs[1]->file.dirent.d_name) >= 0) & 1;
				break ;
			case R_TIME:
				cont = (tm[0] >= tm[1]) & 1;
				break ;
			default:
				break ;
		}
		if (cont == 1)
		{
			rs[0] = rs[0]->next;
			rs[1] = rs[0]->next;
		}
		else
		{
			tmp = rs[0]->file;
			rs[0]->file = rs[1]->file;
			rs[1]->file = tmp;
		}
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
