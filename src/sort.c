#include "ls.h"

int		cmp(const void *a, const void *b)
{
	return strcmp(*(const char**)a, *(const char**)b);
}

static int	rev_cmp(const void *a, const void *b)
{
	return strcmp(*(const char**)b, *(const char**)a);
}

static int	time_cmp(const void *a, const void *b)
{
	if (*((time_t *)a + 2) == *((time_t *)b + 2))
		return rev_cmp(b, a);
	return *((time_t *)b + 2) - *((time_t *)a + 2);
}

static int	rev_time_cmp(const void *a, const void *b)
{
	if (*((time_t *)a + 2) == *((time_t *)b + 2))
		return cmp(b, a);
	return *((time_t *)a + 2) - *((time_t *)b + 2);
}

void	quicksort(t_path *p, int p_size, t_flags *f)
{
	if (p_size < 2)
		return ;
	if (f->t_flag == true)
	{
		if (f->r_flag == true)
			qsort(p, p_size, sizeof(*p), rev_time_cmp);
		else
			qsort(p, p_size, sizeof(*p), time_cmp);
	}
	else if (f->r_flag == true)
		qsort(p, p_size, sizeof(*p), rev_cmp);
	else
		qsort(p, p_size, sizeof(*p), cmp);
}
