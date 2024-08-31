NAME = pipex
CC = cc
GFLAGS = -Wall -Wextra -Werror
# VFLAGS				=	 \
# 						--track-origins=yes \
# 						--leak-check=full \
# LOGFILE				=	valgrind_result.log
SRCS = pipex.c get_path.c finish_program.c utils.c \
		ft_printf_stderr/format_put_num.c ft_printf_stderr/format_put_letter.c \
		 ft_printf_stderr/ft_printf_utils.c ft_printf_stderr/ft_printf.c \
		./libft/ft_strnstr.c ./libft/ft_split.c ./libft/ft_strlen.c \
		./libft/ft_strjoin.c ./libft/ft_strchr.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

# val:					re
# 						@valgrind $(VFLAGS) ./$(NAME) > $(LOGFILE) 2>&1

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
