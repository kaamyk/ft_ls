#include "../inc/ft_ls.h"

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
	int		len_to_keep = ft_strrchr(oldpath, '/') - oldpath;
	char	*n_path = ft_calloc(len_to_keep + ft_strlen(to_add) + 1, 1); // length old path + length to_add + NULL byte

	ft_strlcpy(n_path, oldpath, len_to_keep);
	ft_memcpy(n_path + len_to_keep, to_add, ft_strlen(to_add));
	printf("path_update_file result => %s\n", n_path);
	return (n_path);
}