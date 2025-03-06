# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 13:37:58 by yrafai            #+#    #+#              #
#    Updated: 2025/03/06 17:57:50 by yrafai           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

ifeq ($(shell uname -s), Darwin)
RL_PATH = $(shell brew --prefix readline)
endif

LIB = -L $(RL_PATH)/lib -lreadline

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address -I includes -I$(RL_PATH)/include

OBJSDIR = objs/

SRCS = src/main.c \
src/lexer/lexer.c \
src/lexer/lexer_helpers.c \
src/lexer/lexer_helpers_2.c \
src/lexer/lexer_helpers_3.c \
src/lexer/token_lists.c \
src/lexer/errors/error_msgs.c \
src/lexer/exit_status/exit_status.c \
src/parser/parser.c \
src/parser/ast.c \
src/parser/free_ast.c \
src/parser/ast_helpers.c \
src/parser/ast_helpers_2.c \
src/parser/rdp.c \
src/execution/executor.c \
src/execution/executor_helpers.c \
src/execution/executor_helpers_2.c \
src/execution/executor_helpers_3.c \
src/execution/executor_helpers_4.c \
src/execution/executor_helpers_5.c \
src/expander/expander.c \
src/expander/expander_helpers.c \
src/expander/expander_helpers_2.c \
src/expander/expander_helpers_3.c \
src/expander/expander_helpers_4.c \
src/expander/str_lists.c \
src/expander/str_lists_2.c \
src/expander/str_lists_3.c \
src/expander/str_lists_4.c \
src/expander/wildcard.c \
src/here-doc/heredoc.c \
src/here-doc/heredoc_helpers.c \
src/here-doc/heredoc_helpers_2.c \
src/here-doc/heredoc_helpers_3.c \
src/signals/signal_handler.c \
src/builtin/checker.c \
src/builtin/cd/ft_cd.c \
src/builtin/cd/ft_cd_helpers.c \
src/builtin/cd/ft_cd_helpers_2.c \
src/builtin/echo/ft_echo.c \
src/builtin/exit/ft_exit.c \
src/builtin/export/ft_export.c \
src/builtin/export/ft_export_helpers.c \
src/builtin/export/ft_export_helpers_2.c \
src/builtin/pwd/ft_pwd.c \
src/builtin/unset/ft_unset.c \
src/env/env.c \
src/env/env_2.c \
src/env/env_3.c \
src/env/env_lists.c \
src/builtin/env/ft_env.c

OBJS = $(SRCS:%.c=$(OBJSDIR)%.o)

LIBFT = libft/libft.a

all: $(OBJSDIR) $(LIBFT) $(NAME)

$(LIBFT):
	@echo "Building libft..."
	@make -C libft

$(OBJSDIR):
	@mkdir -p $(OBJSDIR)
	@mkdir -p $(OBJSDIR)/src
	@mkdir -p $(OBJSDIR)/src/lexer
	@mkdir -p $(OBJSDIR)/src/lexer/errors
	@mkdir -p $(OBJSDIR)/src/lexer/exit_status
	@mkdir -p $(OBJSDIR)/src/parser
	@mkdir -p $(OBJSDIR)/src/execution
	@mkdir -p $(OBJSDIR)/src/expander
	@mkdir -p $(OBJSDIR)/src/here-doc
	@mkdir -p $(OBJSDIR)/src/signals
	@mkdir -p $(OBJSDIR)/src/builtin
	@mkdir -p $(OBJSDIR)/src/builtin/cd
	@mkdir -p $(OBJSDIR)/src/builtin/echo
	@mkdir -p $(OBJSDIR)/src/builtin/exit
	@mkdir -p $(OBJSDIR)/src/builtin/export
	@mkdir -p $(OBJSDIR)/src/builtin/pwd
	@mkdir -p $(OBJSDIR)/src/builtin/unset
	@mkdir -p $(OBJSDIR)/src/builtin/env
	@mkdir -p $(OBJSDIR)/src/env

$(NAME): $(OBJS) $(LIBFT)
	@echo "Building minishell..."
	@$(CC) $(OBJS) $(CFLAGS) -o $(NAME) -L libft -lft $(LIB)

$(OBJSDIR)%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning object files..."
	@rm -rf $(OBJSDIR)
	@make clean -C libft

fclean: clean
	@echo "Cleaning executables..."
	@rm -f $(NAME)
	@make fclean -C libft

re: fclean all

.PHONY: all clean fclean re
