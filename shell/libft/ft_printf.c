/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:26:11 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/03 16:01:09 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static const char	*process_format_string(
	const char *format, va_list arg_list, int *out_len)
{
	t_format	formats;

	formats = init_format();
	format = set_specifications(format, &formats);
	if (formats.conv == 'c')
		print_char(
			(char)va_arg(arg_list, int), formats, out_len);
	else if (formats.conv == 's')
		print_string(
			va_arg(arg_list, char *), formats, out_len);
	else if (formats.conv == 'p')
		print_hex(
			va_arg(arg_list, size_t), formats, out_len);
	else if (formats.conv == 'x' || formats.conv == 'X')
		print_hex(
			va_arg(arg_list, unsigned int), formats, out_len);
	else if (formats.conv == 'd' || formats.conv == 'i')
		print_int(
			va_arg(arg_list, int), formats, out_len);
	else if (formats.conv == 'u')
		print_int(
			va_arg(arg_list, unsigned int), formats, out_len);
	else
		print_char(formats.conv, formats, out_len);
	return (++format);
}

int	ft_printf(const char *format, ...)
{
	va_list	arg_list;
	int		output_size;

	output_size = 0;
	if (write(1, 0, 0) == -1)
		return (-1);
	va_start(arg_list, format);
	while (*format)
	{
		if (*format == '%')
		{
			if (*(format + 1) == '\0')
				break ;
			format = process_format_string(format + 1, arg_list, &output_size);
		}
		else
			write_char((char *)format++, &output_size);
		if (output_size == -1)
		{
			va_end(arg_list);
			return (-1);
		}
	}
	va_end(arg_list);
	return (output_size);
}
