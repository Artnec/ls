#include "ls.h"

static void	exit_illegal_option(char *prog_name, char option)
{
	fprintf(stderr, "%s: illegal option -- %c\n", prog_name, option);
	fprintf(stderr, "usage: %s [-AGRUacdfglortu1] [file ...]\n", prog_name);
	exit(1);
}

static int	is_1fgo(char c, t_flags *f)
{
	if (c == '1')
	{
		f->one_flag = true;
		f->l_flag = false;
	}
	else if (c == 'f')
	{
		f->a_flag = true;
		f->f_flag = true;
	}
	else if (c == 'g')
	{
		f->l_flag = true;
		f->g_flag = true;
	}
	else if (c == 'o')
	{
		f->l_flag = true;
		f->o_flag = true;
	}
	else
		return false;
	return true;
}

static int	is_lUuc(char c, t_flags *f)
{
	if (c == 'l')
	{
		f->l_flag = true;
		f->one_flag = false;
	}
	else if (c == 'U')
	{
		f->U_flag = true;
		f->u_flag = false;
		f->c_flag = false;
	}
	else if (c == 'u')
	{
		f->u_flag = true;
		f->U_flag = false;
		f->c_flag = false;
	}
	else if (c == 'c')
	{
		f->c_flag = true;
		f->u_flag = false;
		f->U_flag = false;
	}
	else
		return false;
	return true;
}

int			parse_flags(int argc, char **argv, t_flags *f)
{
	int n = 1;
	for (; n < argc; n++)
	{
		if (argv[n][0] != '-' || argv[n][1] == '\0')
			break ;
		for (int i = 1; argv[n][i] != '\0'; i++)
		{
			if (argv[n][i] == 'R')
				f->R_flag = true;
			else if (argv[n][i] == 'a')
				f->a_flag = true;
			else if (argv[n][i] == 'A')
				f->A_flag = true;
			else if (argv[n][i] == 'r')
				f->r_flag = true;
			else if (argv[n][i] == 't')
				f->t_flag = true;
			else if (argv[n][i] == 'G')
				f->G_flag = true;
			else if (argv[n][i] == 'd')
				f->d_flag = true;
			else if (!is_1fgo(argv[n][i], f) && !is_lUuc(argv[n][i], f))
				exit_illegal_option(argv[0], argv[n][i]);
		}
	}
	if (!isatty(fileno(stdout)))
		f->output_is_redirected = true;
	return n;
}
