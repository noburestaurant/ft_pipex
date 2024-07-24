NAME = pipex
CC = cc
GFLAGS = -Wall -Wextra -Werror
SRCS = pipex.c get_path.c error.c libft.c ft_printf/format_put_letter.c \
		ft_printf/format_put_num.c ft_printf/ft_printf_utils.c ft_printf/ft_printf.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re