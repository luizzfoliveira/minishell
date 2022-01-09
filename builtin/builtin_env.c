/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 01:09:06 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/03 11:17:22 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_env(char **envp, t_cmds *cmds)
{
	int	fd;
	int	i;

	if (cmds->fd_out == 0)
		fd = 1;
	else if (cmds->fd_out == 1000)
		fd = 0;
	else
		fd = cmds->fd_out;
	i = -1;
	while (envp[++i])
	{
		write(fd, envp[i], ft_strlen(envp[i]));
		write(fd, "\n", 1);
	}
	exit(0);
}
