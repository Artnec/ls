NAME		=	ft_ls

CC			=	clang

FLAGS		=	-O -Wall -Werror -Wextra -Wshadow -Wpedantic

SRC			=	main.c	parse_flags.c	print_in_long_format.c \
				print_entry_name.c	print_only_names.c	get_dir_content.c \
				print_arguments.c	sort.c

HEADER		=	ls.h

OBJ			=	$(addprefix $(OBJ_DIR), $(SRC:.c=.o))
INC			=	$(addprefix $(SRC_DIR), $(HEADER))

SRC_DIR		=	src/
OBJ_DIR		=	obj/


all: $(NAME)

obj:
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(INC) | obj
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
