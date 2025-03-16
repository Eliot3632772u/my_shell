/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_manager_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yrafai <yrafai@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:28:14 by yrafai            #+#    #+#             */
/*   Updated: 2025/03/16 17:28:24 by yrafai           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	reset_default_sig_handlers(void)
{
	g_last_signal = 0;
	handle_default_sig_handlers(RESET);
}

void	install_default_sig_handlers(void)
{
	g_last_signal = 0;
	handle_default_sig_handlers(SET);
}
