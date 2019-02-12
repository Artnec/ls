#include "ls.h"

static void	print_content(t_path *p, int p_size, t_flags *f)
{
	if (p_size < 1)
		return;
	if (f->f_flag == false && p_size > 1)
		quicksort(p, p_size, f);

	if (f->l_flag == true)
		print_in_long_format(p, p_size, f);
	else
		print_only_names(p, p_size, f);
}

static void	print_dir_content(char *dir_path, t_flags *f)
{
	int p_size = 0;
	t_path *p = get_dir_content(dir_path, &p_size, f);
	if (p == NULL)
		return;
	print_content(p, p_size, f);
	if (f->R_flag == true)
	{
		for (int i = 0; i < p_size; i++)
		{
			if (!strcmp(p[i].name, ".") || !strcmp(p[i].name, ".."))
				continue;

			if (S_ISDIR(p[i].st_mode))
			{
				printf("\n%s:\n", p[i].path);
				print_dir_content(p[i].path, f);
			}
		}
	}
	for (int i = 0; i < p_size; i++)
		free(p[i].path);
	free(p);
}

static void	swap(t_path *a, t_path *b)
{
	t_path tmp;
	memcpy(&tmp, a, sizeof(t_path));
	memcpy(a, b, sizeof(t_path));
	memcpy(b, &tmp, sizeof(t_path));
}

static void	print_directories(t_path *p, int p_size, int dir_count, t_flags *f)
{
	f->type = DIRECTORY;
	quicksort(p, dir_count, f);
	if (dir_count > 0)
	{
		if (f->one_argument == false)
			printf("%s%s:\n", dir_count == p_size ? "" : "\n", p[0].path);
		print_dir_content(p[0].path, f);
	}
	for (int i = 1; i < dir_count; i++)
	{
		printf("\n%s:\n", p[i].path);
		print_dir_content(p[i].path, f);
	}
}

void		print_arguments(t_path *p, int p_size, t_flags *f)
{
	if (f->d_flag == true)
	{
		print_content(p, p_size, f);
		return;
	}

	int dir_count = 0;
	for (int e = p_size - 1; dir_count < e;)
	{
		while (dir_count < e && S_ISDIR(p[dir_count].st_mode))
			dir_count++;
		while (dir_count < e && !S_ISDIR(p[e].st_mode))
			e--;
		swap(p + dir_count, p + e);
	}
	if (S_ISDIR(p[dir_count].st_mode))
		dir_count++;
	f->type = FILES;
	print_content(p + dir_count, p_size - dir_count, f);
	print_directories(p, p_size, dir_count, f);
}
