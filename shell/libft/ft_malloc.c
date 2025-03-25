/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 00:02:40 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/25 10:24:58 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"

void	*ft_malloc(unsigned long size)
{
	void	*ptr;

	if (size == 0)
	{
		write(2, "Minishell: Cannot allocate 0 bytes.\n", 37);
		exit(69);
	}
	ptr = malloc(size);
	if (!ptr)
	{
		write(2, "Minishell: MALLOC FAILED!!\n", 28);
		exit(69);
	}
	return (ptr);
}
