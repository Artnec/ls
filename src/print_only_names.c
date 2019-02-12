#include "ls.h"

static int	get_padding_size(t_path *p, int p_size, t_flags *f)
{
	int max = 0;
	for (int i = 0; i < p_size; i++)
	{
		if (max < p[i].name_size)
			max = p[i].name_size;
	}
	if (f->G_flag == true)
		return max + 1;
	return max + (8 - max % 8);
}

void		print_only_names(t_path *p, int p_size, t_flags *f)
{
	if (f->output_is_redirected == true || f->one_flag == true)
	{
		for (int i = 0; i < p_size; i++)
			printf("%s\n", p[i].name);
	}
	else
	{
		struct winsize w;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		int padding_size = get_padding_size(p, p_size, f);

		int entries_per_row = w.ws_col / padding_size;
		if (entries_per_row == 0)
			entries_per_row = 1;

		int rows = p_size / entries_per_row;
		if (p_size != rows * entries_per_row)
			rows++;

		for (int n = 0; n < rows; n++)
		{
			for (int i = n; i < p_size; i += rows)
			{
				print_entry_name(p + i, f);
				printf("%*s", padding_size - p[i].name_size, "");
			}
			printf("\n");
		}
	}
}
