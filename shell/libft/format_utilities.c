/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:31:57 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/01 09:31:59 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	add_sign(
	long number, t_format format, int *out_len)
{
	if (number < 0)
		write_char("-", out_len);
	else if (format.plus && format.conv != 'u')
		write_char("+", out_len);
	else if (format.space && format.conv != 'u')
		write_char(" ", out_len);
}

int	is_valid_flag(char c)
{
	return (c == '-' || c == '0' || c == '.'
		|| c == '#' || c == ' ' || c == '+');
}

int	is_valid_formatifier(char c)
{
	return (c);
}

void	write_hex(
	size_t number, char format, int *out_len)
{
	char	*base;

	base = "0123456789abcdef";
	if (format == 'X')
		base = "0123456789ABCDEF";
	if (number >= 16)
		write_hex(number / 16, format, out_len);
	write_char(&base[number % 16], out_len);
}

void	write_number(long number, int *out_len)
{
	char	digit;

	if (number < 0)
		number = -number;
	if (number >= 10)
		write_number(number / 10, out_len);
	digit = (number % 10) + '0';
	write_char(&digit, out_len);
}
