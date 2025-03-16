# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 13:37:58 by yrafai            #+#    #+#              #
#    Updated: 2025/03/16 14:38:58 by irabhi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

ifeq ($(shell uname -s), Darwin)
RL_PATH = $(shell brew --prefix readline)
endif

LIB = -L $(RL_PATH)/lib -lreadline

CFLAGS = -Wall -Wextra -Werror -I includes -I$(RL_PATH)/include -g -fsanitize=address

OBJSDIR = objs/

SRCS = src/main/minishell.c \
src/main/minishell_helpers.c \
src/main/prompt_handler.c \
src/lexer/lexer_core.c \
src/lexer/token_analyzer.c \
src/lexer/lexer2.c \
src/lexer/lexer2_utils.c \
src/lexer/lexer2_utils3.c \
src/parser/parser2.c \
src/parser/parser2_utils.c \
src/execution/exec_redc.c \
src/execution/exec_redc_utils.c\
src/execution/exec_redc_utils_2.c\
src/lexer/lexer2_utils2.c\
src/parser/parser2_utils2.c\
src/parser/parser2_checks.c\
src/lexer/token_validation.c \
src/lexer/token_creation.c \
src/lexer/token_manipulation.c \
src/lexer/errors/error_msgs.c \
src/lexer/exit_status/exit_status.c \
src/parser/parser_core.c \
src/parser/ast_builder.c \
src/parser/ast_cleanup.c \
src/parser/ast_node_operations.c \
src/parser/ast_validation.c \
src/parser/recursive_descent_parser.c \
src/execution/arg_processing.c \
src/execution/dir_handling.c \
src/execution/command_exec.c \
src/execution/error_handling.c \
src/execution/execution.c \
src/execution/file_execution.c \
src/execution/path_utils.c \
src/execution/pipe_and_logic_exec.c \
src/execution/process_management.c \
src/execution/redir_handling.c \
src/expander/argv_handling.c \
src/expander/command_processing.c \
src/expander/env_expansion.c \
src/expander/env_var_helpers.c \
src/expander/expansion_utils.c \
src/expander/quote_handling.c \
src/expander/split_utils.c \
src/expander/str_list_sorting.c \
src/expander/str_list_utils.c \
src/expander/str_utils_helpers.c \
src/expander/wildcard_expansion.c \
src/here-doc/heredoc_core.c \
src/here-doc/heredoc_io.c \
src/here-doc/heredoc_signals.c \
src/here-doc/heredoc_utils.c \
src/here-doc/heredoc_utils_2.c \
src/here-doc/heredoc_cleanup.c \
src/signals/signal_manager.c \
src/builtin/builtin_dispatcher.c \
src/builtin/cd/ft_cd.c \
src/builtin/cd/ft_cd_utils.c \
src/builtin/cd/path_resolution.c \
src/builtin/echo/ft_echo.c \
src/builtin/exit/ft_exit.c \
src/builtin/export/ft_export.c \
src/builtin/export/ft_export_utils.c \
src/builtin/pwd/ft_pwd.c \
src/builtin/unset/ft_unset.c \
src/builtin/env/ft_env.c \
src/environment/env_core.c \
src/environment/env_lists.c \
src/environment/env_variables.c \
src/environment/env_utils.c \

OBJS = $(SRCS:%.c=$(OBJSDIR)%.o)

LIBFT = libft/libft.a

all: $(OBJSDIR) $(LIBFT) $(NAME)

$(LIBFT):
	@echo "Building libft..."
	@make -C libft

$(OBJSDIR):
	@mkdir -p $(OBJSDIR)
	@mkdir -p $(OBJSDIR)/src/main
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
	@mkdir -p $(OBJSDIR)/src/environment

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
