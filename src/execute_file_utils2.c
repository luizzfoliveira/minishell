/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file_utils2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:28 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 20:01:53 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	open_file(char *argv, int i)
{
	int	file;

	file = 0;
	if (i == 0)
		file = open(argv, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else if (i == 1)
		file = open(argv, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	else if (i == 2)
		file = open(argv, O_RDONLY, 0777);
	return (file);
}

void	run_aux(t_cmds *cmds, pid_t pid, int *fd, t_sig **act)
{
	int	status;

	close(fd[1]);
	if (cmds->fd_in > 0)
		reset_input();
	if (cmds->fd_out == 1000)
		dup2(fd[0], STDIN_FILENO);
	waitpid(pid, &status, 0);
	if (g_reset_fd[2] != 130 && g_reset_fd[2] != 131)
		g_reset_fd[2] = WEXITSTATUS(status);
	close(fd[0]);
	if (cmds->fd_out != 1000 && cmds->fd_out != 0)
	{
		reset_input();
		reset_output();
	}
	if (cmds->fd_out == 0)
		reset_input();
	config_sigaction((void *)act, SIG_IGN, SIGQUIT);
	config_sigaction((void *)act, sigint_handle, SIGINT);
}

char	*init_run(char **envp, t_cmds *cmds, int fd[], t_sig **act)
{
	char	*file_path;

	file_path = 0;
	if (access(cmds->cmd, X_OK) == 0)
		file_path = ft_strdup(cmds->cmd);
	else if (cmds->cmd[0] != '.')
		file_path = find_path(cmds->cmd, envp);
	if (!file_path)
		return (0);
	config_sigaction((void *)act, handle_sigquit, SIGQUIT);
	config_sigaction((void *)act, sigint_handle_cmd, SIGINT);
	pipe(fd);
	return (file_path);
}

void	if_else(t_cmds *cmds, int fd[])
{
	if (cmds->fd_out == 0)
		reset_output();
	else if (cmds->fd_out == 1000)
		dup2(fd[1], STDOUT_FILENO);
	else
		dup2(cmds->fd_out, STDOUT_FILENO);
}

int	run(char **argv, char **envp, t_cmds *cmds, t_sig **act)
{
	pid_t	pid;
	char	*file_path;
	int		fd[2];

	file_path = init_run(envp, cmds, fd, act);
	if (!file_path)
		return (0);
	pid = fork();
	if (pid == 0)
	{
		if (cmds->fd_in > 0)
			dup2(cmds->fd_in, STDIN_FILENO);
		close(fd[0]);
		if_else(cmds, fd);
		if (execve(file_path, argv, envp) == -1)
			no_file(file_path);
		exit(errno);
	}
	else
		run_aux(cmds, pid, fd, act);
	free(file_path);
	return (0);
}
