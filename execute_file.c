/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:18:59 by felipe            #+#    #+#             */
/*   Updated: 2021/12/07 18:52:53 by felipe           ###   ########.fr       */
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
	pid_t	pid;
	int		fd[2];
	int 	red;

	int	out; 
	out = 10;

	if (pipe(fd) == -1)
		error();
	pid = fork();
	if (pid == -1)
		error();
	if(pid == 0)
	{
		if (out == 0)
			close(fd[1]);
		else if (out == 10)
			dup2(fd[1], STDOUT_FILENO); //redirecionar para um pipe
		else if (out == 20)
		{
			red = open_file("outfile", 0);
			dup2(red, STDOUT_FILENO);
		}	
		else if (out == 21)
		{
			red = open_file("outfile", 1);
			dup2(red, STDOUT_FILENO);
		}
		close(fd[0]);
		execve(file_path, argv, envp);
	}
	else
	{
		if (out == 0)
			close(fd[0]);
		close(fd[1]);
		waitpid(pid, NULL, 0);
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