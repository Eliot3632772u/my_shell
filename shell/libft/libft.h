/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irabhi <irabhi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 00:27:40 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/15 17:33:43 by irabhi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>
# include <signal.h>
# include <stdint.h>
# include <limits.h>
# include <fcntl.h>
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif
// # define OPEN_MAX 10000

typedef struct s_list
{
	long			number;
	long			index;
	void			*content;
	struct s_list	*next;
	struct s_list	*prev;
}					t_list;

void			*ft_malloc(unsigned long size);
bool			ft_isspecial(char c);
bool			str_contains(char *str, char c);
bool			ft_isspace(char c);
int				ft_strcmp(char *s1, char *s2);
int				ft_atoi_2(const char *str);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_isprint(int c);
size_t			ft_strlen(const char *s);
void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
void			*ft_memcpy(void *dst, const void *src, size_t n);
void			*ft_memmove(void *dst, const void *src, size_t len);
size_t			ft_strlcpy(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
int				ft_toupper(int c);
int				ft_tolower(int c);
char			*ft_strchr(const char *s, int c);
char			*ft_strrchr(const char *s, int c);
int				ft_strncmp(const char *s1, const char *s2, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
char			*ft_strnstr(const char *haystack, \
	const char *needle, size_t len);
int				ft_atoi(const char *str);
void			*ft_calloc(size_t count, size_t size);
char			*ft_strdup(const char *s1);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strtrim(char const *s1, char const *set);
char			**ft_split(char const *s, char c);
char			*ft_itoa(int n);
char			*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
void			ft_putchar_fd(char c, int fd);
void			ft_putstr_fd(char *s, int fd);
void			ft_putendl_fd(char *s, int fd);
void			ft_putnbr_fd(int n, int fd);
t_list			*ft_lstnew(void *content);
void			ft_lstadd_front(t_list **lst, t_list *new);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
void			ft_lstadd_back(t_list **lst, t_list *new);
void			ft_lstdelone(t_list *lst, void (*del)(void *));
void			ft_lstclear(t_list **lst, void (*del)(void *));
void			ft_lstiter(t_list *lst, void (*f)(void *));
t_list			*ft_lstmap(t_list *lst, void *(*f)(void *), \
	void (*del)(void *));
char			*get_next_line(int fd);
void			*clean(char **ptr);
char			*ft_strappend(char **s1, const char *s2);
void			ft_error_msg(void);
void			alpha_check(char **argv);
void			clean_2(char **ptr);
void			ft_free(t_list **lst);
t_list			*ft_stack_new(int content);

typedef struct s_format
{
	size_t	width;
	int		precision;
	int		plus;
	int		align;
	int		prefix;
	int		space;
	char	pad_char;
	char	conv;
}	t_format;

typedef struct s_vars
{
	size_t	size;
	size_t	precision;
	size_t	pad;
}	t_vars;

int				ft_printf(const char *format, ...);
int				is_valid_formatifier(char c);
int				is_valid_flag(char c);
void			write_char(char *character, int *out_len);
t_format		init_format(void);
const char		*set_specifications(const char *str, \
					t_format *format);
void			print_char(char character, t_format format, \
					int *out_len);
void			print_string(char *str, t_format format, \
					int *out_len);
void			print_hex(size_t number, t_format format, \
					int *out_len);
void			write_hex(size_t number, char format, \
					int *out_len);
void			write_number(long number, int *out_len);
void			print_int(long number, t_format format, \
					int *out_len);
int				count_decimal(long number);
int				count_hex(size_t number);
void			add_sign(long number, t_format format, \
					int *out_len);
void			put_repeat(char c, size_t count, int *out_len);

// stringbuilder
typedef struct s_stringbuilder
{
	size_t	capacity;
	size_t	used_len;
	char	*str;
}	t_strbuilder;

t_strbuilder	*stringbuilder(void);
t_strbuilder	*sb_append(t_strbuilder *sb, char *str);
t_strbuilder	*sb_append_free(t_strbuilder *sb, char *str);
t_strbuilder	*sb_append_int(t_strbuilder *sb, int nb);
void			sb_free(t_strbuilder *sb);
void			sb_reallocate(t_strbuilder *sb, size_t len);

#endif
