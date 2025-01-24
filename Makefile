CC          := cc
CFLAGS      := -Wall -Wextra -Werror -g -fsanitize=address
LDFLAGS     := -Llibft -lft

# OS-specific settings for readline
UNAME_S     := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)  # macOS
    READLINE_FLAGS := -lreadline
else                       # Linux
    READLINE_FLAGS := -lreadline -lncurses
endif

FLAGS_L = -I$(HOME)/.brew/opt/readline/include

READLINE = -L$(HOME)/.brew/opt/readline/lib

SRC_DIR     := src
INCLUDE_DIR := includes
BUILTIN_DIR := $(SRC_DIR)/builtin
EXECUTOR_DIR:= $(SRC_DIR)/executor
LEXER_DIR   := $(SRC_DIR)/lexer
PARSER_DIR  := $(SRC_DIR)/parser
MAIN_DIR    := $(SRC_DIR)/main
LIBFT_DIR   := libft

SRC_FILES   := $(wildcard $(MAIN_DIR)/*.c) \
               $(wildcard $(LEXER_DIR)/*.c) \
               $(wildcard $(BUILTIN_DIR)/*.c) \
               $(wildcard $(EXECUTOR_DIR)/*.c)
               #$(wildcard $(PARSER_DIR)/*.c) 
OBJ_FILES   := $(SRC_FILES:.c=.o)

NAME        := minishell

all: $(NAME)

$(NAME): $(OBJ_FILES)
	@$(MAKE) -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) $(OBJ_FILES) $(LDFLAGS) $(READLINE_FLAGS) ${READLINE} -o $(NAME)
	@echo "Built $(NAME) successfully!"

%.o: %.c
	@$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIBFT_DIR) ${FLAGS_L} -c $< -o $@

clean:
	@rm -f $(OBJ_FILES)
	@$(MAKE) clean -C $(LIBFT_DIR)
	@echo "Cleaned object files."

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "Removed $(NAME)."

re: fclean all

.PHONY: all clean fclean re
