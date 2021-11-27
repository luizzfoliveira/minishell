/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:18:59 by felipe            #+#    #+#             */
/*   Updated: 2021/11/27 19:51:23 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	len_list(t_args *list)
{
	t_args	*iter;
	int		i;

	i = 0;
	iter = list;
	while (iter)
	{
		i++;
		iter = iter->next;
	}
}

/* Function that will look for the path line inside the environment, will
 split and test each command path and then return the right one. */
char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		i++;
	}
	return (0);
}

void	run_execve(char *file_path, char **argv, char **envp)
{
	pid_t	child;
	int		status;

	child = fork();
	if(child == 0)
	{
		execve(file_path, argv, envp);
	}
	else
	{
		waitpid(child, &status, 0);
	}
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute(t_cmds *cmds, char **envp)
{
	t_vars	*v_iter;
	t_args	*iter;
	char	**argv;
	int		i;

	argv = ft_calloc(len_list(cmds->args) + 1, sizeof (char *));
	argv[0] = ft_strdup(cmds->cmd);
	i = 1;
	iter = cmds->args;
	while (iter)
	{
		argv[i] = iter->arg;
		i++;
		iter = iter->next;
	}
	if (access(cmds->cmd, X_OK) == 0)
		run_execve(cmds->cmd, argv, envp);
	else if (find_path(cmds->cmd, envp))
		run_execve(find_path(cmds->cmd, envp), argv, envp);
	else if (access(cmds->cmd, F_OK) == -1)
		printf("%s: No such file or directory\n", cmds->cmd);
	else
		printf("%s: Permission denied\n", cmds->cmd);
	return (0);
	/* if (execve(find_path(cmd[0], envp), cmd, envp) == -1) */
		/*retornar um erro*/;
}