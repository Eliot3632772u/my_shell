/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 09:30:02 by yrafai            #+#    #+#             */
/*   Updated: 2024/12/01 09:30:03 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	count_hex(size_t number)
{
	int	count;

	count = 0;
	if (number == 0)
		return (1);
	while (number)
	{
		number /= 16;
		count++;
	}
	return (count);
}

int	count_decimal(long number)
{
	int	count;

	count = 0;
	if (number == 0)
		count++;
	while (number != 0)
	{
		number /= 10;
		count++;
	}
	return (count);
}
