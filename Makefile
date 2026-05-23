CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude
RM = rm -f

NAME = codexion

SRCS = src/main.c src/parsing.c src/initialize.c src/safe.c\
		src/coding.c src/set_get.c src/sync.c src/data_op.c src/analyser.c\
		src/dongle_sync.c src/coding_utils.c src/safe_cond.c \
		src/end_condition.c  src/global_queue.c  src/global_queue_utils.c \
		src/heap/heap_utils.c src/heap/heap_ops.c src/heap/heap_cmp.c\
		src/heap/global_init.c\
		src/utils/cleanup.c src/utils/coder_util.c src/utils/dongle_util.c\
		src/utils/time_utils.c

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