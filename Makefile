CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude
RM = rm -f

NAME = codexion

SRCS = src/main.c src/end_utils.c src/parsing.c src/initialize.c src/safe.c\
		src/coding.c src/set_get.c src/sync.c src/data_op.c src/analyser.c\
		src/time_utils.c src/dongle_sync.c src/heap/heap_ops.c src/heap/heap_cmp.c\
		src/heap/heap_utils.c src/coding_utils.c src/arbiter.c src/safe_cond.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

norm:
	norminette $(SRCS) include/codexion.h

fc: fclean

re: fclean all

.PHONY: all clean fclean re norm