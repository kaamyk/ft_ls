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
}

int8_t	set_env(t_ftls *data)
{
	get_uid(data);
	return (data->userid);
}