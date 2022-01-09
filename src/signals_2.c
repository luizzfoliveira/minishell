/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 21:01:01 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/07 12:38:17 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigquit(int sig)
{
	(void)sig;
	g_reset_fd[2] = 42;
	write(0, "\b\b  \b\b", 6);
}

/*
void	heredoc_sigquit(int sig)
{
	(void)sig;
	printf("Minihell: warning: here-document delimited");
	printf("by end-of-file(wanted `eof')\n");
	g_reset_fd[2] = 130;
	exit(130);
}*/
