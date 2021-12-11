/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:47:06 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 01:57:03 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	sig_handler(int sig, siginfo_t *info, void *ucontext)
{
	if (sig == SIGINT)
	{
		write(0, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
	(void)info;
	(void)(ucontext);
}

void	recieve_signals(void)
{
	struct sigaction	int_signal;
	struct sigaction	quit_signal;
	pid_t				pid;

	int_signal.sa_sigaction = sig_handler;
	int_signal.sa_flags = SA_SIGINFO;
	quit_signal.sa_handler = SIG_IGN;
	pid = getpid();
	(void)pid;
	if (sigemptyset(&int_signal.sa_mask) == -1)
	{
		write(1, "Erro\n", 5);
		return ;
	}
	if (sigaction(SIGINT, &int_signal, NULL) == -1)
	{
		write(1, "Erro\n", 5);
		return ;
	}
	if (sigaction(SIGQUIT, &quit_signal, NULL) == -1)
	{
		write(1, "Erro\n", 5);
		return ;
	}
}
