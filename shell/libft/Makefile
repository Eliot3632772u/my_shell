# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yrafai <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/22 00:46:45 by yrafai            #+#    #+#              #
#    Updated: 2024/11/01 02:35:37 by yrafai           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRCS = string_builder.c string_builder_utils.c ft_malloc.c ft_isspace.c ft_isspecial.c ft_strcmp.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_isprint.c ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strrchr.c ft_tolower.c ft_toupper.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c char_writer.c format_count_utils.c format_parser.c format_utilities.c ft_printf.c handle_format.c print_helpers.c printf_helper_herlper.c get_next_line_bonus.c clean.c ft_strappend.c ft_error_msg.c alpha_check.c ft_stack_new.c ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

OBJS = $(SRCS:.c=.o)

CC = cc

HEADER = libft.h

CFLAGS = -Wall -Wextra -Werror

NAME = libft.a

all: $(NAME)

$(OBJS) $(BONUS_OBJS): $(HEADER)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

clean:
	rm -rf $(OBJS) $(BONUS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
