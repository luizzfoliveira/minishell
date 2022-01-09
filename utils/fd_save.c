/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_save.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:27:14 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/19 19:42:05 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_origin_fd(void)
{
	g_reset_fd[0] = dup(0);
	g_reset_fd[1] = dup(1);
	g_reset_fd[2] = 0;
}

void	reset_input(void)
{
	dup2(g_reset_fd[0], 0);
}

void	reset_output(void)
{
	dup2(g_reset_fd[1], 1);
}

int	exec_no_file(t_cmds *cmds)
{
	printf("%s: No such file or directory\n", cmds->cmd);
	reset_input();
	reset_output();
	g_reset_fd[2] = 127;
	return (0);
}

int	exec_no_perm(t_cmds *cmds)
{	
	printf("%s: Permission denied\n", cmds->cmd);
	reset_input();
	reset_output();
	g_reset_fd[2] = 126;
	return (0);
}
