# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 13:37:58 by yrafai            #+#    #+#              #
#    Updated: 2025/03/11 10:51:06 by yrafai           ###   ########.fr        #
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
src/main/prompt_handler.c \
src/lexer/lexer_core.c \
src/lexer/token_analyzer.c \
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
src/execution/executor_core.c \
src/execution/command_executor.c \
src/execution/pipeline_executor.c \
src/execution/redirection_handler.c \
src/execution/process_manager.c \
src/execution/execution_utils.c \
src/execution/pipe_operations.c \
src/execution/file_operations.c \
src/execution/environment_setup.c \
src/execution/cleanup_operations.c \
src/expander/expander_core.c \
src/expander/variable_expansion.c \
src/expander/path_expansion.c \
src/expander/quote_handling.c \
src/expander/expansion_utils.c \
src/expander/wildcard_expansion.c \
src/expander/string_manipulation.c \
src/expander/string_utils.c \
src/expander/string_lists.c \
src/expander/string_operations.c \
src/expander/pattern_matching.c \
src/here-doc/heredoc_processor.c \
src/here-doc/heredoc_io.c \
src/here-doc/heredoc_signals.c \
src/here-doc/heredoc_utils.c \
src/here-doc/heredoc_cleanup.c \
src/signals/signal_manager.c \
src/builtin/builtin_dispatcher.c \
src/builtin/cd/directory_change.c \
src/builtin/cd/path_resolution.c \
src/builtin/cd/directory_utils.c \
src/builtin/echo/echo_handler.c \
src/builtin/exit/exit_handler.c \
src/builtin/export/export_handler.c \
src/builtin/export/variable_operations.c \
src/builtin/export/export_utils.c \
src/builtin/pwd/pwd_handler.c \
src/builtin/unset/unset_handler.c \
src/builtin/env/env_display.c \
src/env/env_core.c \
src/env/env_variables.c \
src/env/env_utils.c \
src/env/env_list_operations.c

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
