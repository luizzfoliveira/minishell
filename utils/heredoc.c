/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 21:22:55 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/08 11:06:01 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	mini_gnl_sigquit(void)
{
	write(1, "\nminihell: warning: here-document ", 35);
	write(1, "delimited by end-of-file (wanted `eof')\n", 41);
	rl_replace_line("", 0);
	rl_on_new_line();
	exit(0);
}

int	mini_gnl(char **line)
{
	char	c;
	int		i;
	int		r;

	i = -1;
	(void) r;
	write(1, "> ", 2);
	while (1)
	{
		g_reset_fd[2] = 0;
		i++;
		r = read(0, &c, 1);
		line[0][i] = c;
		if (c == '\n')
			break ;
		if (c == 0 && line[0][0] == 0)
		{
			if (g_reset_fd[2] != 42)
				mini_gnl_sigquit();
		}
	}
	return (1);
}

int	heredoc_child(t_sig **act, t_cmds *cmds, int *fd, int pid)
{
	int	status;

	close(fd[1]);
	cmds->fd_in = fd[0];
	config_sigaction((void *)act, SIG_IGN, SIGINT);
	config_sigaction((void *)act, SIG_IGN, SIGQUIT);
	waitpid(pid, &status, 0);
	config_sigaction((void *)act, SIG_IGN, SIGQUIT);
	config_sigaction((void *)act, sigint_handle, SIGINT);
	if (g_reset_fd[2] == 42)
		g_reset_fd[2] = 0;
	if (g_reset_fd[2] != 130 && g_reset_fd[2] != 131)
		g_reset_fd[2] = WEXITSTATUS(status);
	return (g_reset_fd[2]);
}

int	limiter_cmp(char *s1, char *s2)
{
	size_t	i;

	if ((ft_strlen(s1) - 1) != ft_strlen(s2))
		return (1);
	i = 0;
	while (s2[i] != 0)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	here_doc(char *limiter, t_sig **act, t_cmds *cmds)
{
	pid_t	pid;
	int		fd[2];
	char	*line;

	config_sigaction((void *)act, heredoc_sigint, SIGINT);
	config_sigaction((void *)act, heredoc_sigquit, SIGQUIT);
	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		line = ft_calloc(1000, sizeof(char));
		close(fd[0]);
		while (mini_gnl(&line))
		{
			if (limiter_cmp(line, limiter) == 0)
				break ;
			write(fd[1], line, ft_strlen(line));
			free(line);
			line = ft_calloc(1000, sizeof(char));
		}
		free(line);
		exit(0);
	}
	else
		return (heredoc_child(act, cmds, fd, pid));
}
