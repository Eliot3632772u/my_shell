/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:30:34 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/01 09:30:35 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	put_repeat(char c, size_t count, int *out_len)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		write_char(&c, out_len);
		if (*out_len == -1)
			return ;
		i++;
	}
}

t_vars	calculate_vars(long number, t_format fmt)
{
	t_vars	vars;

	vars.size = count_decimal(number);
	vars.precision = 0;
	vars.pad = 0;
	if (fmt.precision != -1 && (size_t)fmt.precision > vars.size)
		vars.precision = fmt.precision - vars.size;
	if (fmt.precision == 0 && number == 0)
		vars.size = 0;
	if ((fmt.plus || fmt.space || number < 0) && fmt.conv != 'u')
		vars.size++;
	if (!fmt.align && (fmt.pad_char == ' ' || fmt.precision != -1)
		&& fmt.width > vars.size + vars.precision)
		vars.pad = fmt.width - (vars.size + vars.precision);
	return (vars);
}

void	handle_padding_and_sign(
	long number, t_format fmt, t_vars vars, int *out_len)
{
	put_repeat(' ', vars.pad, out_len);
	add_sign(number, fmt, out_len);
	if (!fmt.align && fmt.pad_char == '0' && fmt.precision == -1
		&& fmt.width > vars.size + vars.precision)
		put_repeat('0', fmt.width - (vars.size + vars.precision), out_len);
	if (fmt.precision != -1)
		put_repeat('0', vars.precision, out_len);
	if (fmt.precision != 0 || number != 0)
		write_number(number, out_len);
	if (fmt.align && fmt.width > vars.size + vars.precision)
		put_repeat(' ', fmt.width - (vars.size + vars.precision), out_len);
}

void	print_int(long number, t_format fmt, int *out_len)
{
	t_vars	vars;

	vars = calculate_vars(number, fmt);
	handle_padding_and_sign(number, fmt, vars, out_len);
}
