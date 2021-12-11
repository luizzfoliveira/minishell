/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 22:58:18 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 01:52:20 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_cmds  *iter)
{
	t_args	*i;

	i = iter->args;
	while (i)
	{
		if (i->arg)
			write(1, i->arg, ft_strlen(i->arg));
		if (i->next)
			write(1, " ", 1);
		i = i->next;
	}
	if (!iter->flags)
		write(1, "\n", 1);
}
