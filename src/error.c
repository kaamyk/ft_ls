#include "../inc/ft_ls.h"

void	print_err(int err)
{
	char *err_mes = ft_strjoin("ft_ls : ", strerror(err));
	write(2, err_mes, ft_strlen(err_mes));
	write(2, "\n", 1);
	free(err_mes);
}