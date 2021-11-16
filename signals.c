/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:47:06 by felipe            #+#    #+#             */
/*   Updated: 2021/11/14 12:57:27 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
	sigaction(SIGINT, &int_signal, NULL);
	/* if (sigaction(SIGINT, &int_signal, NULL) == 0)
		printf("oi\n"); */
}
