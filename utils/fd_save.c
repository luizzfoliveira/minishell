/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_save.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 11:27:14 by lbricio-          #+#    #+#             */
/*   Updated: 2021/12/10 16:44:07 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	save_origin_fd()
{
	g_reset_fd[0] = dup(0);
	g_reset_fd[1] = dup(1);
	g_reset_fd[2] = 0;
}

void	reset_input()
{
	dup2(g_reset_fd[0], 0);
	/*close(g_reset_fd[0]);*/
}

void	reset_output()
{
	dup2(g_reset_fd[1], 1);
	/*close(g_reset_fd[1]);*/
}

char	*status_itoa()
{
	char *ret;

	if(g_reset_fd[2] == 0)
		ret = "0";
	if(g_reset_fd[2] == 1)
		ret = "1";
	if(g_reset_fd[2] == 2)
		ret = "2";
	if(g_reset_fd[2] == 126)
		ret = "126";
	if(g_reset_fd[2] == 127)
		ret = "127";
	if(g_reset_fd[2] == 128)
		ret = "128";
	if(g_reset_fd[2] == 130)
		ret = "130";
	return(ret);
}
