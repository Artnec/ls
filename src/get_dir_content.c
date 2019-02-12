#include "ls.h"

#define MAX 128

static void		add_entry(t_path *p, char *entry, char *dir_path, t_flags *f)
{
	p->name_size = strlen(entry);
	p->path = malloc(strlen(dir_path) + p->name_size + 2);
	int i = 0;
	for (; dir_path[i] != '\0'; i++)
		p->path[i] = dir_path[i];
	p->path[i++] = '/';
	p->name = p->path + i;
	for (int n = 0; entry[n] != '\0'; n++, i++)
		p->path[i] = entry[n];
	p->path[i] = '\0';

	struct stat stat_buf;
	lstat(p->path, &stat_buf);
	p->st_mode = stat_buf.st_mode;
	add_sorting_priority(p, &stat_buf, f);
}

static t_path	*double_array_capacity(t_path *p, int *max)
{
	*max *= 2;
	t_path *ret = malloc(sizeof(*ret) * *max);
	for (int i = 0; i < *max / 2; i++)
		ret[i] = p[i];
	free(p);
	return ret;
}

t_path			*get_dir_content(char *dir_path, int *p_size, t_flags *f)
{
	DIR *dir = opendir(dir_path);
	if (dir == NULL)
	{
		fprintf(stderr, "%s: ", f->prog_name);
		perror(dir_path);
		return NULL;
	}

	int max = MAX;
	t_path *p = malloc(sizeof(*p) * max);

	struct dirent *dp;
	while ((dp = readdir(dir)) != NULL)
	{
		if (dp->d_name[0] == '.' && f->A_flag != true && f->a_flag != true)
			continue;
		if (f->a_flag == false && (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")))
			continue;
		if (*p_size == max)
			p = double_array_capacity(p, &max);
		add_entry(p + (*p_size)++, dp->d_name, dir_path, f);
	}
	closedir(dir);
	return p;
}
