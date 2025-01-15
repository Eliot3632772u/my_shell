SRC = main.c utils/push_front.c utils/free.c libft/ft_putendl_fd.c \
	libft/ft_putstr_fd.c libft/ft_split.c libft/ft_strdup.c libft/ft_strlen.c \
	libft/ft_putchar_fd.c libft/ft_strlcpy.c libft/ft_strjoin.c libft/ft_substr.c

SRC_O = ${SRC:.c=.o}

FLAGS = -Wall -Wextra -Werror

BIN = minishell

FLAGS_L = -I$(HOME)/.brew/opt/readline/include

READLINE = -L$(HOME)/.brew/opt/readline/lib

all: ${BIN}

${BIN}: ${SRC_O}
	cc -g -fsanitize=address ${READLINE} -l readline ${SRC_O} -o ${BIN}

%.o: %.c minishell.h
	cc ${FLAGS} ${FLAGS_L} -c $< -o $@

clean:
	rm -f ${SRC_O}

fclean: clean
	rm -f ${BIN}

re: fclean all