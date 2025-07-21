#include "../inc/ft_ls.h"

void	get_entrie_err(int err)
{
	char *err_mes = ft_strjoin("ft_ls : ", strerror(err));
	write(2, err_mes, strlen(err_mes));
	write(2, "\n", 1);
	free(err_mes);
}