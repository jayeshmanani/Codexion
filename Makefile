CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -Iinclude
RM = rm -f

NAME = codexion

SRCS = src/main.c src/utils.c src/parsing.c src/initialize.c src/safe.c\
		src/coding.c src/set_get.c src/sync.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re