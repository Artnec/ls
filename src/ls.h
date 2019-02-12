#ifndef LS_H
# define LS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pwd.h>
# include <grp.h>
# include <sys/stat.h>
# include <dirent.h>
# include <time.h>
# include <sys/acl.h>
# include <sys/xattr.h>
# include <string.h>
# include <stdbool.h>
# include <sys/ioctl.h>

enum e_types {FILES, DIRECTORY};

typedef struct		s_flags
{
	char			*prog_name;
	uint8_t			l_flag;
	uint8_t			R_flag;
	uint8_t			a_flag;
	uint8_t			A_flag;
	uint8_t			r_flag;
	uint8_t			t_flag;
	uint8_t			f_flag;
	uint8_t			g_flag;
	uint8_t			G_flag;
	uint8_t			o_flag;
	uint8_t			d_flag;
	uint8_t			u_flag;
	uint8_t			U_flag;
	uint8_t			c_flag;
	uint8_t			one_flag;
	uint8_t			output_is_redirected;
	uint8_t			type;
	uint8_t			one_argument;
}					t_flags;

typedef struct		s_lformat
{
	int				total;
	int				links_padding;
	int				size_padding;
	int				name_padding;
	int				group_padding;
}					t_lformat;

typedef struct		s_path
{
	char			*name;
	char			*path;
	time_t			sort_time;
	int				name_size;
	short			st_mode;
}					t_path;


void				quicksort(t_path *p, int p_size, t_flags *f);
int					parse_flags(int argc, char **argv, t_flags *f);
int					cmp(const void *a, const void *b);
void				print_only_names(t_path *p, int p_size, t_flags *f);
void				print_in_long_format(t_path *p, int p_size, t_flags *f);
void				print_entry_name(t_path *p, t_flags *f);
t_path				*get_dir_content(char *dir_path, int *p_size, t_flags *f);
void				add_sorting_priority(t_path *p, struct stat *stat_buf, t_flags *f);
void				print_arguments(t_path *p, int p_size, t_flags *f);

#endif
