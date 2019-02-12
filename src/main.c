#include "ls.h"

void	add_sorting_priority(t_path *p, struct stat *stat_buf, t_flags *f)
{
	if (f->u_flag == true)
		p->sort_time = stat_buf->st_atime;
	else if (f->c_flag == true)
		p->sort_time = stat_buf->st_ctime;
	else if (f->U_flag == true)
		p->sort_time = stat_buf->st_birthtime;
	else
		p->sort_time = stat_buf->st_mtime;
}

void	print_errors(char **err_files, int err_size, char *prog_name)
{
	if (err_size > 1)
		qsort(err_files, err_size, sizeof(*err_files), cmp);
	for (int i = 0; i < err_size; i++)
		fprintf(stderr, "%s: %s: No such file or directory\n", prog_name, err_files[i]);
}

void	process_arguments(char **args, int arg_num, t_flags *f)
{
	f->one_argument = arg_num == 1 ? true : false;
	t_path p[arg_num];
	int p_size = 0;
	char *err_files[arg_num];
	int err_size = 0;
	for (int i = 0; i < arg_num; i++)
	{
		struct stat stat_buf;
		int err = f->l_flag ? lstat(args[i], &stat_buf) : stat(args[i], &stat_buf);
		if (err == -1)
		{
			err_files[err_size++] = args[i];
			continue;
		}
		p[p_size].path = args[i];
		p[p_size].name = args[i];
		p[p_size].name_size = strlen(args[i]);
		p[p_size].st_mode = stat_buf.st_mode;
		add_sorting_priority(p + p_size, &stat_buf, f);
		p_size++;
	}
	print_errors(err_files, err_size, f->prog_name);
	print_arguments(p, p_size, f);
}

int		main(int argc, char **argv)
{
	t_flags f;
	memset(&f, 0, sizeof(f));

	f.prog_name = argv[0];
	int arg_idx = parse_flags(argc, argv, &f);
	if (arg_idx == argc)
	{
		char *cur_dir = ".";
		process_arguments(&cur_dir, 1, &f);
	}
	else
		process_arguments(argv + arg_idx, argc - arg_idx, &f);
	return 0;
}
