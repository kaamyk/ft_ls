#include "../inc/ft_ls.h"

char	*format_path(char **oldpath)
{
	char	*last_sl = NULL;
	char	*tmp_str = NULL;
	int		len = 0;

	last_sl = ft_strrchr(*oldpath, '/');
	if (last_sl != NULL)
	{
		if (*(last_sl - 1) == '/')
		{
			while (last_sl != *oldpath && *last_sl == '/')
				--last_sl;
		}
		tmp_str = ft_calloc(last_sl - *oldpath + 2, 1);
		ft_strlcpy(tmp_str, *oldpath, last_sl - *oldpath + 2);
		*oldpath = tmp_str;
	}
	else
	{
		len = ft_strlen(*oldpath);
		tmp_str = ft_calloc(len + 2, 1);
		ft_strlcpy(tmp_str, *oldpath, len + 1);
		tmp_str[len] = '/';
		free(*oldpath);
		*oldpath = tmp_str;
	}
	return (*oldpath);
}

char	*path_update_subdir(char *oldpath, char *to_add)
{
	char	*tmp = NULL;
	
	if (to_add != NULL)
	{
		if (oldpath != NULL)
		{
			format_path(&oldpath);
			tmp = ft_strjoin(oldpath, "/");
			if (tmp == NULL)
			{
				print_err(errno);
				return (NULL);
			}
			free(oldpath);
		}
		oldpath = ft_strjoin(tmp, to_add);
	}
	if (oldpath == NULL)
		print_err(errno);
	if (tmp != NULL)
		free(tmp);
	return (oldpath);
}

char	*path_update_file(char *oldpath, char *to_add)
{
	char	*last_sep = ft_strrchr(oldpath, '/');
	unsigned int len_to_keep = last_sep - oldpath + 1;
	char	*n_path = ft_calloc(len_to_keep + ft_strlen(to_add) + 1, 1); // length old path + length to_add + NULL byte

	ft_strlcpy(n_path, oldpath, len_to_keep + 1);
	ft_memcpy(n_path + len_to_keep, to_add, ft_strlen(to_add));
	free(oldpath);
	return (n_path);
}