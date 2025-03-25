# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/06 13:37:58 by yrafai            #+#    #+#              #
#    Updated: 2025/03/25 10:16:56 by yrafai           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN=\033[0;32m
CYAN=\033[0;36m
YELLOW=\033[0;33m
RED=\033[0;31m
NC=\033[0m

NAME = minishell

LIB = -lreadline

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address

OBJSDIR = objs/

SRCS = \
	libft/string_builder.c \
	libft/string_builder_utils.c \
	libft/ft_malloc.c \
	libft/ft_isspace.c \
    libft/ft_isspecial.c \
	libft/ft_strcmp.c \
	libft/ft_bzero.c \
	libft/ft_isalnum.c \
	libft/ft_isalpha.c \
    libft/ft_isascii.c \
	libft/ft_isdigit.c \
	libft/ft_isprint.c \
	libft/ft_memcpy.c \
	libft/ft_memmove.c \
    libft/ft_memset.c \
	libft/ft_strchr.c \
	libft/ft_strlcat.c \
	libft/ft_strlcpy.c \
	libft/ft_strlen.c \
    libft/ft_strrchr.c \
	libft/ft_tolower.c \
	libft/ft_toupper.c \
	libft/ft_strncmp.c \
	libft/ft_memchr.c \
    libft/ft_memcmp.c \
	libft/ft_strnstr.c \
	libft/ft_atoi.c \
	libft/ft_calloc.c \
	libft/ft_strdup.c \
    libft/ft_substr.c \
	libft/ft_strjoin.c \
	libft/ft_strtrim.c \
	libft/ft_split.c \
	libft/ft_itoa.c \
    libft/ft_strmapi.c \
	libft/ft_striteri.c \
	libft/ft_putchar_fd.c \
	libft/ft_putstr_fd.c \
    libft/ft_putendl_fd.c \
	libft/ft_putnbr_fd.c \
	libft/char_writer.c \
	libft/format_count_utils.c \
    libft/format_parser.c \
	libft/format_utilities.c \
	libft/ft_printf.c \
	libft/handle_format.c \
    libft/print_helpers.c \
	libft/printf_helper_herlper.c \
	libft/get_next_line_bonus.c \
    libft/clean.c \
	libft/ft_strappend.c \
	libft/ft_error_msg.c \
	libft/alpha_check.c \
	libft/ft_stack_new.c \
    libft/ft_lstnew_bonus.c \
	libft/ft_lstadd_front_bonus.c \
	libft/ft_lstsize_bonus.c \
    libft/ft_lstlast_bonus.c \
	libft/ft_lstadd_back_bonus.c \
	libft/ft_lstdelone_bonus.c \
    libft/ft_lstclear_bonus.c \
	libft/ft_lstiter_bonus.c \
	libft/ft_lstmap_bonus.c \
    src/main/minishell.c \
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
    src/execution/exec_redc_utils.c \
    src/execution/exec_redc_utils_2.c \
    src/lexer/lexer2_utils2.c \
    src/parser/parser2_utils2.c \
    src/parser/parser2_checks.c \
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
    src/here-doc/heredoc_utils_3.c \
    src/here-doc/heredoc_cleanup.c \
    src/signals/signal_manager.c \
    src/signals/signal_manager_2.c \
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
    src/environment/env_utils.c

OBJS = $(SRCS:%.c=$(OBJSDIR)%.o)

all: $(OBJSDIR) $(NAME)

$(OBJSDIR):
	@mkdir -p $(OBJSDIR)
	@mkdir -p $(OBJSDIR)/libft
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

$(NAME): $(OBJS)
	@echo "${GREEN}Building minishell...${NC}"
	@$(CC) $(OBJS) $(CFLAGS) -o $(NAME) $(LIB)

$(OBJSDIR)%.o: %.c includes/minishell.h includes/libft.h
	@echo "${CYAN}Compiling $<...${NC}"
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "${YELLOW}Cleaning object files...${NC}"
	@rm -rf $(OBJSDIR)

fclean: clean
	@echo "${RED}Cleaning executables...${NC}"
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
