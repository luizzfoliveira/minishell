/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:49 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 00:24:11 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	sigint_handle_cmd(int sig)
{
	(void)sig;
	printf("\n");
	g_reset_fd[2] = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
	reset_input();
	reset_output();
	printf("quit (code dumped)\n");
	g_reset_fd[2] = 131;
}

void	sigint_handle(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		g_reset_fd[2] = 130;
	}
}

void	heredoc_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	g_reset_fd[2] = 130;
	exit(130);
}

void	config_sigaction(struct sigaction *act, void (*handler)(int), int sig)
{
	act->sa_handler = handler;
	act->sa_flags = 0;
	sigemptyset(&act->sa_mask);
	sigaction(sig, act, NULL);
}
