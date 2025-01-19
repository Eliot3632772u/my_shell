/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_helper_herlper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 15:47:39 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/04 15:47:40 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	print_char(
	char character, t_format format, int *out_len)
{
	size_t	padding;

	padding = 0;
	if (format.width > 1)
		padding = format.width - 1;
	if (!format.align)
		put_repeat(format.pad_char, padding, out_len);
	write_char(&character, out_len);
	if (format.align)
		put_repeat(' ', padding, out_len);
}

void	print_string(
	char *str, t_format format, int *out_len)
{
	size_t	padding;
	size_t	length;
	size_t	i;

	padding = 0;
	length = 0;
	if (!str)
		str = "(null)";
	length = ft_strlen(str);
	if (format.precision != -1 && (size_t)format.precision < length)
		length = format.precision;
	if (format.width > length)
		padding = format.width - length;
	if (!format.align)
		put_repeat(format.pad_char, padding, out_len);
	i = 0;
	while (i < length)
	{
		write_char(&str[i], out_len);
		i++;
	}
	if (format.align)
		put_repeat(' ', padding, out_len);
}

void	print_hex(
		size_t nbr, t_format fmt, int *out_len)
{
	size_t	size;

	size = count_hex(nbr);
	if (fmt.precision != -1 && (size_t)fmt.precision > size)
		size = fmt.precision;
	if (fmt.precision == 0 && nbr == 0)
		size = 0;
	if (fmt.conv == 'p' || (fmt.prefix && nbr != 0))
		size += 2;
	if (!fmt.align && (fmt.pad_char == ' ' || fmt.precision != -1)
		&& fmt.width > size)
		put_repeat(' ', fmt.width - size, out_len);
	if (fmt.conv == 'p' || (fmt.conv == 'x' && fmt.prefix && nbr && size != 0))
		*out_len += write(1, "0x", 2);
	else if (fmt.conv == 'X' && fmt.prefix && nbr && size != 0)
		*out_len += write(1, "0X", 2);
	if (fmt.precision > count_hex(nbr))
		put_repeat('0', fmt.precision - count_hex(nbr), out_len);
	if (!fmt.align && fmt.pad_char == '0' && fmt.width > size
		&& fmt.precision == -1)
		put_repeat('0', fmt.width - size, out_len);
	if ((fmt.conv == 'p' && size != 2) || (fmt.conv != 'p' && size != 0))
		write_hex(nbr, fmt.conv, out_len);
	if (fmt.align && fmt.width > size)
		put_repeat(' ', fmt.width - size, out_len);
}
