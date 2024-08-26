NAME = pipex
CC = cc -g
GFLAGS = -Wall -Wextra -Werror
SRCS = pipex.c get_path.c finish_program.c utils.c \
		ft_printf_stderr/format_put_num.c ft_printf_stderr/format_put_letter.c \
		 ft_printf_stderr/ft_printf_utils.c ft_printf_stderr/ft_printf.c \
		./libft/ft_strnstr.c ./libft/ft_split.c ./libft/ft_strlen.c \
		./libft/ft_strjoin.c ./libft/ft_strchr.c
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
