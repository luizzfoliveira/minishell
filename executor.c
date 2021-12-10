/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/09 22:07:18 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_env(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		printf("%s\n", envp[i]);
}

/* if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", ft_strlen(iter->cmd)))
			exec_env(*envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "echo", ft_strlen(iter->cmd)))
			ft_echo(iter);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "exit", ft_strlen(iter->cmd)))
			builtin_exit(iter, variables);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "export", ft_strlen(iter->cmd)))
			builtin_export(iter, variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "unset", ft_strlen(iter->cmd)))
			builtin_unset(iter, variables, envp); */


int	run_command(t_cmds *cmds, char **envp, int fd_in, int fd_out, int i)
{
	if (dup2(fd_in, STDIN_FILENO) == -1)
		printf("error\n");
	if (dup2(fd_out, STDOUT_FILENO) == -1)
		printf("error\n");
	close(fd_in);
	execute(cmds, envp);
}

void	executor(t_data *data, t_vars *variables, char ***envp)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;
	char	*line;
	int		fd[2];
	pid_t	pid;
	int		status;

	iter = data->cmds;
	int	i = 0;
	while (iter)
	{
		pipe(fd);
		pid = fork();
		if (pid == -1)
			printf("error\n");
		else if (pid == 0)
		{
			if (!iter->next)
				run_command(iter, *envp, data->file_in, data->file_out, i);
			else
				run_command(iter, *envp, data->file_in, fd[1], i);
		}
		else
		{
			wait(NULL);
			close(fd[1]);
			data->file_in = fd[0];
			iter = iter->next;
			i++;
		}
	}
	close(data->file_out);
}
