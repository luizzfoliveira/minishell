/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:18:59 by felipe            #+#    #+#             */
/*   Updated: 2021/12/09 19:10:35 by felipe           ###   ########.fr       */
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

void	error(void)
{
	perror("\033[31mError");
	exit(EXIT_FAILURE);
}

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
	if (file == -1)
		error();
	return (file);
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

char	**cmds_to_argv(t_cmds *cmds)
{
	t_args	*iter;
	char	**argv;
	int		size;
	int		i;

	size = len_list(cmds->args);
	if (cmds->flags)
		size++;
	argv = ft_calloc(size + 1, sizeof (char *));
	argv[0] = ft_strdup(cmds->cmd);
	i = 1;
	if (cmds->flags)
		argv[i++] = ft_strdup(cmds->flags);
	iter = cmds->args;
	while (iter)
	{
		argv[i] = iter->arg;
		i++;
		iter = iter->next;
	}
	return (argv);
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute(t_cmds *cmds, char **envp)
{
	t_vars	*v_iter;
	char	**argv;
	int		i;

	argv = cmds_to_argv(cmds);
	if (access(cmds->cmd, X_OK) == 0)
		execve(cmds->cmd, argv, envp);
	else if (find_path(cmds->cmd, envp))
		execve(find_path(cmds->cmd, envp), argv, envp);
	else if (access(cmds->cmd, F_OK) == -1)
		printf("%s: No such file or directory\n", cmds->cmd);
	else
		printf("%s: Permission denied\n", cmds->cmd);
	return (0);
	/* if (execve(find_path(cmd[0], envp), cmd, envp) == -1) */
		/*retornar um erro*/
}