/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:21:06 by felipe            #+#    #+#             */
/*   Updated: 2021/12/12 18:34:07 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	len_list(t_args *list)
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

char	*change_directory(char *path)
{
	char	*slash;
	char	*final;
	char	*file;

	slash = ft_strchr(path, '/');
	while (slash)
	{
		final = slash;
		slash = ft_strchr(slash + 1, '/');
	}
	file = final + 1;
	*final = 0;
	chdir(path);
	*final = '/';
	return (file);
}

static void	run_file(char *file_path, char **argv, char **envp)
{
	/*printf("out inside execve: %i\n", out);*/
	pid_t	pid;
	char	*file;
	int		fd[2];
	int		status;

	pid = fork();
	if(pid == 0)
	{
		file = change_directory(file_path);
		if (access(file, X_OK) == 0)
			execve(file, argv, envp);
		else if (access(file, F_OK) == -1)
			printf("%s: No such file or directory\n", file_path);
		else
			printf("%s: Permission denied\n", file_path);
	}
	else
	{
		waitpid(pid, &status, 0);
	}
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute_file(t_cmds *cmds, char **envp)
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
	run_file(cmds->cmd, argv, envp);
	return (0);
}