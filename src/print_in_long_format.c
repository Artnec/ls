#include "ls.h"

#define HALF_YEAR 15770000

static int	num_len(int n)
{
	int len = 1;
	while (n /= 10)
		len++;
	return len;
}

static void	get_paddings_and_total(t_lformat *l, t_path *p, int p_size, t_flags *f)
{
	int size_max = 0;
	int links_max = 0;
	l->total = 0;
	l->name_padding = 0;
	l->group_padding = 0;
	for (int i = 0; i < p_size; i++)
	{
		struct stat stat_buf;
		lstat(p[i].path, &stat_buf);
		if (stat_buf.st_nlink > links_max)
			links_max = stat_buf.st_nlink;
		if (stat_buf.st_size > size_max)
			size_max = stat_buf.st_size;

		struct passwd *pw = getpwuid(stat_buf.st_uid);
		int tmp = strlen(pw->pw_name) + 1;
		if (tmp > l->name_padding)
			l->name_padding = tmp;

		struct group  *gr = getgrgid(stat_buf.st_gid);
		tmp = strlen(gr->gr_name) + 1;
		if (tmp > l->group_padding)
			l->group_padding = tmp;
		l->total += stat_buf.st_blocks;
	}
	l->links_padding = num_len(links_max) + 1;
	l->size_padding = num_len(size_max) + 1;
	if (f->o_flag == true && f->g_flag == true)
		l->size_padding += 2;
}

static void	print_type_and_permissions(short st_mode)
{
	if (S_ISREG(st_mode))
		printf("-");
	else if (S_ISDIR(st_mode))
		printf("d");
	else if (S_ISBLK(st_mode))
		printf("b");
	else if (S_ISCHR(st_mode))
		printf("c");
	else if (S_ISSOCK(st_mode))
		printf("s");
	else if (S_ISLNK(st_mode))
		printf("l");
	else if (S_ISFIFO(st_mode))
		printf("p");
	for (int i = 6; i >= 0; i -= 3)
	{
		printf("%c", (st_mode >> i) & 4 ? 'r' : '-');
		printf("%c", (st_mode >> i) & 2 ? 'w' : '-');
		printf("%c", (st_mode >> i) & 1 ? 'x' : '-');
	}
}

static void	print_acl_or_extended_attributes(char *path)
{
	acl_entry_t tmp;
	acl_t acl = acl_get_link_np(path, ACL_TYPE_EXTENDED);
	if (acl)
	{
		if (acl_get_entry(acl, ACL_FIRST_ENTRY, &tmp) == -1)
			acl = NULL;
		acl_free(acl);
	}

	if (listxattr(path, NULL, 0, XATTR_NOFOLLOW) > 0)
		printf("@");
	else if (acl != NULL)
		printf("+");
	else
		printf(" ");
}

static void	print_user_and_group_name(uid_t user_id, gid_t group_id, t_lformat *l, t_flags *f)
{
	if (f->g_flag == false)
	{
		struct passwd *pw = getpwuid(user_id);
		printf(" %*s", -l->name_padding, pw->pw_name);
	}
	if (f->o_flag == false)
	{
		struct group  *gr = getgrgid(group_id);
		printf(" %*s", -l->group_padding, gr->gr_name);
	}
}

static void	print_last_access_or_mod_time(time_t change_time)
{
	time_t cur_time = time(NULL);
	char *date = ctime(&change_time) + 4;
	if (cur_time >= change_time + HALF_YEAR)
		printf("%.6s  %.4s ", date, date + 16);
	else
		printf("%.12s ", date);
}

void		print_in_long_format(t_path *p, int p_size, t_flags *f)
{
	t_lformat l;
	get_paddings_and_total(&l, p, p_size, f);

	if (f->d_flag == false && f->type == DIRECTORY)
		printf("total %d\n", l.total);
	for (int i = 0; i < p_size; i++)
	{
		struct stat stat_buf;
		lstat(p[i].path, &stat_buf);
		print_type_and_permissions(stat_buf.st_mode);
		print_acl_or_extended_attributes(p[i].path);
		printf("%*d", l.links_padding, stat_buf.st_nlink);
		print_user_and_group_name(stat_buf.st_uid, stat_buf.st_gid, &l, f);
		printf("%*lld ", l.size_padding, stat_buf.st_size);
		print_last_access_or_mod_time(p[i].sort_time);
		print_entry_name(p + i, f);
		if (S_ISLNK(stat_buf.st_mode))
		{
			char buf[128] = {0};
			readlink(p[i].path, buf, 128);
			printf(" -> %s", buf);
		}
		printf("\n");
	}
}
