/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_format.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:29:22 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/01 09:29:23 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	get_formatifier(const char *str)
{
	while (is_valid_flag(*str) || ft_isdigit(*str))
		str++;
	if (is_valid_formatifier(*str))
		return (*str);
	return (0);
}

t_format	init_format(void)
{
	t_format	format;

	format.width = 0;
	format.precision = -1;
	format.plus = 0;
	format.align = 0;
	format.prefix = 0;
	format.space = 0;
	format.pad_char = ' ';
	format.conv = 0;
	return (format);
}

static const char	*parse_precision(const char *str, t_format *format)
{
	int	i;

	if (!ft_isdigit(*(str + 1)))
	{
		format->precision = 0;
		return (str);
	}
	i = 1;
	while (*(str + i) == '0')
		i++;
	format->precision = ft_atoi(str + i);
	if (format->precision == 0)
		str += i - 1;
	else
		str += count_decimal(format->precision) + i - 1;
	return (str);
}

static const char	*parse_format_flags(const char *str, t_format *format)
{
	if (*str == '.')
		str = parse_precision(str, format);
	else if (*str == '0')
		format->pad_char = '0';
	else if (*str == '-')
		format->align = 1;
	else if (*str == ' ')
		format->space = 1;
	else if (*str == '#')
		format->prefix = 1;
	else if (*str == '+')
		format->plus = 1;
	else if (ft_isdigit(*str))
	{
		format->width = ft_atoi(str);
		str += count_decimal(format->width) - 1;
	}
	str++;
	return (str);
}

const char	*set_specifications(const char *str, t_format *format)
{
	format->conv = get_formatifier(str);
	if (!format->conv)
	{
		while (is_valid_flag(*(str)) || ft_isdigit(*(str)))
			str++;
		return (str);
	}
	while (*(str) && *str != format->conv)
		str = parse_format_flags(str, format);
	return (str);
}
