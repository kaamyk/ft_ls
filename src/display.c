#include "../inc/ft_ls.h"
void	display_type(t_file_list *entry)
{
	if (entry->file.dirent.d_type == DT_DIR)
		write(STDOUT_FILENO, "d", 1);
	else
		write(STDOUT_FILENO, "-", 1);
}

void	display_rights(t_file_list *entry)
{
	uint8_t	shifter = 8;
	
	for (uint8_t i = 0; i < 9; i++)
	{
		if ((entry->file.stat.st_mode >> shifter) & 1)
		{
			switch (shifter % 3)
			{
				case 0:
				write(STDOUT_FILENO, "x", 1);
				break;
				case 1:
				write(STDOUT_FILENO, "w", 1);
				break;
				case 2:
				write(STDOUT_FILENO, "r", 1);
					break;
			}
		}
		else
			write(STDOUT_FILENO, "-", 1);
		--shifter;
	}
	write(STDOUT_FILENO, " ", 1);
}

void	display_group_user_name(t_file_list *entry, uint8_t long_format_data[4])
{
	struct passwd	*us = getpwuid(entry->file.stat.st_uid);
	struct group	*gr = getgrgid(entry->file.stat.st_gid);

	ft_printf("%*s %*s ", long_format_data[0], us->pw_name, long_format_data[1], gr->gr_name);
}

void	display_date(t_file_list *entry)
{
	time_t	raw_time = entry->file.stat.st_mtim.tv_sec; 
	char	*str = ctime(&raw_time);
	
	ft_printf("%.12s ", str + 4);
}

void	display_long_format(t_file_list *entry, uint8_t long_format_data[4])
{
	display_type(entry);
	display_rights(entry);
	ft_printf("%.*ld ", long_format_data[3],entry->file.stat.st_nlink);
	display_group_user_name(entry, long_format_data);
	ft_printf("%*ld ", long_format_data[2], entry->file.stat.st_size);
	display_date(entry);
	write(STDOUT_FILENO, entry->file.dirent.d_name, ft_strlen(entry->file.dirent.d_name));
	write(STDOUT_FILENO, "\n", 1);
}

void	get_long_format_data(t_file_list *entries, uint8_t long_format_data[4], uint32_t *total_blocks, const bool list_all)
{
	t_file_list	*runner = entries;
	int			tmp = 0;
	struct passwd	*us = NULL;
	struct group	*gr = NULL;

	while (runner != NULL)
	{
		if ((list_all == 0 && runner->file.dirent.d_name[0] != '.')
			|| (list_all == 1))
		{
			us = getpwuid(runner->file.stat.st_uid);
			gr = getgrgid(runner->file.stat.st_gid);
			if (ft_strlen(us->pw_name) > long_format_data[0])
				long_format_data[0] = ft_strlen(us->pw_name);
			if ((tmp = ft_strlen(gr->gr_name)) > long_format_data[1])
				long_format_data[1] = tmp;
			if ((tmp = ft_nblen(runner->file.stat.st_size)) > long_format_data[2])
				long_format_data[2] = tmp;
			if ((tmp = ft_nblen(runner->file.stat.st_nlink)) > long_format_data[3])
				long_format_data[3] = tmp;
			*total_blocks += runner->file.stat.st_blocks;
		}
		runner = runner->next;
	}
}

void	display(t_file_list *entries, bool long_format, bool list_all)
{
	t_file_list	*runner 	= entries;
	uint8_t		long_format_data[4] = {0};		// 0: username, 1: groupanme, 2: size, 3: nb link
	uint32_t	total_blocks = 0;

	if (long_format == 1)
	{
		get_long_format_data(entries, long_format_data, &total_blocks, list_all);
		ft_printf("total %d\n", total_blocks / 2); // Divide by 2 => stat.st_blocks counts 512 bytes blocks. GNU ls count 1K block. So 2 times bigger.
	}
	while (runner != NULL)
	{
		if (list_all == 1 || runner->file.dirent.d_name[0] != '.')
		{
			if (long_format == 1)
				display_long_format(runner, long_format_data);
			else
				ft_printf("%s  ", runner->file.dirent.d_name);
		}
		runner = runner->next;
	}
	ft_printf("\n");
}

void	print_usage(void)
{
	write(STDOUT_FILENO, "Usage: ft_ls [OPTION] [FILE]\nArguments:\n\t-a : do not ignore entries starting with a .\n\t-l : use a long listing format\n\t-r : reverse order while sorting\n\t-R : list subdirectiories recursively\n\t-t : sort by time, newest first\n", 225);
}