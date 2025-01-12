NAME = pipex

CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = part_of_exec.c pipex.c pipex_utils.c cmd_operation.c Makefile path_operation.c pipex.h error_check.c pipex_utils2.c

OBJS = $(SRCS:.c=.o)

all : $(NAME)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@ ar rcs ${NAME} ${OBJS}

clean:
	@ rm -f $(OBJS)

fclean: clean
	@ rm -f $(NAME)

re: fclean all

all: $(NAME)

.PHONY: clean fclean re all
