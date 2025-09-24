#include "../inc/ft_ls.h"

char	*format_path(char **oldpath)
{
	// char	*last_sep = NULL;
	char	*tmp_str = NULL;
	int		len = ft_strlen(*oldpath);

	if (*oldpath == NULL || (*oldpath)[ft_strlen(*oldpath) - 1] == '/')
	 	return (*oldpath);
	tmp_str = ft_calloc(len + 2, 1);
	ft_strlcpy(tmp_str, *oldpath, len + 1);
	tmp_str[len] = '/';
	free(*oldpath);
	*oldpath = tmp_str;
	return (*oldpath);
}

char	*path_update_subdir(char *oldpath, char *to_add)
{
	char	*tmp = oldpath;
	bool	to_free = 0;
	
	if (to_add != NULL)
	{
		if (oldpath != NULL && oldpath[ft_strlen(oldpath) - 1] != '/')
		{
			to_free = 1;
			tmp = ft_strjoin(oldpath, "/");
			if (tmp == NULL)
			{
				print_err(errno);
				return (NULL);
			}
		}
		oldpath = ft_strjoin(tmp, to_add);
	}
	if (to_free == 1 && tmp != NULL)
		free(tmp);
	if (oldpath == NULL)
		print_err(errno);
	return (oldpath);
}

char	*path_update_file(char *oldpath, const char *to_add)
{
	char	*last_sep = ft_strrchr(oldpath, '/');
	unsigned int len_to_keep = last_sep - oldpath + 1;
	char	*n_path = ft_calloc(len_to_keep + ft_strlen(to_add) + 1, 1); // length old path + length to_add + NULL byte

	if (n_path == NULL)
		return (NULL);
	ft_strlcpy(n_path, oldpath, len_to_keep + 1);
	ft_memcpy(n_path + len_to_keep, to_add, ft_strlen(to_add));
	free(oldpath);
	return (n_path);
}