/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_format.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:31:21 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 10:26:35 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	process_and_print(
	void *value, t_format format, int *out_len)
{
	if (format.conv == '%' || format.conv == 'c')
		print_char((char)(uintptr_t)value, format, out_len);
	else if (format.conv == 's')
		print_string((char *)value, format, out_len);
	else if (format.conv == 'p')
		print_hex((size_t)(uintptr_t)value, format, out_len);
	else if (format.conv == 'x' || format.conv == 'X')
		print_hex((unsigned int)(uintptr_t)value, format, out_len);
	else if (format.conv == 'd' || format.conv == 'i')
		print_int((int)(uintptr_t)value, format, out_len);
	else if (format.conv == 'u')
		print_int((unsigned int)(uintptr_t)value, format, out_len);
}
