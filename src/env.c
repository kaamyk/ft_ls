#include "../inc/ft_ls.h"

void	get_uid(t_ftls *data)
{
	struct stat st = {0};

	if (stat("/proc/self/", &st) == -1)
	{
		print_err(errno);
		data->userid = -1;
		return ;
	}
	data->userid = st.st_uid;
	printf("user -> %d\n", data->userid);
}

void	get_pwd(t_ftls *data, char **env)
{
	while (*env != NULL)
	{
		if (*env[0] == 'P' && ft_strnstr(*env, "PWD=", 4) != NULL)
			break ;
		++env;
	}
	if (*env != NULL)
		data->pwd = *env + 4;
	printf("pwd -> %s\n", data->pwd);
}