#include "ls.h"

static void		print_color(short st_mode)
{
	if (S_ISDIR(st_mode))
		printf("\033[34m");
	else if (S_ISBLK(st_mode))
		printf("\033[46m\033[34m");
	else if (S_ISCHR(st_mode))
		printf("\033[43m\033[34m");
	else if (S_ISSOCK(st_mode))
		printf("\033[32m");
	else if (S_ISLNK(st_mode))
		printf("\033[35m");
	else if (S_ISFIFO(st_mode))
		printf("\033[33m");
	else if (st_mode & 1 || (st_mode >> 3) & 1 || (st_mode >> 6) & 1)
		printf("\033[31m");
}

void			print_entry_name(t_path *p, t_flags *f)
{
	if (f->G_flag == true && f->output_is_redirected == false)
		print_color(p->st_mode);
	printf("%s", p->name);
	if (f->G_flag == true && f->output_is_redirected == false)
		printf("\033[0m");
}
