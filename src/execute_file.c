/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:33 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:26:18 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	copy_flags(t_data *data, char **argv, t_cmds *cmds)
{
	int	i;

	i = 1;
	if (cmds->flags)
	{
		argv[i] = ft_strdup(cmds->flags);
		if (!argv[i])
		{
			while (--i >= 0)
				free(argv[i]);
			free(argv);
			cleanup(data, 2);
		}
		i++;
	}
	return (i);
}

int	size_cmds(t_cmds *cmds)
{
	int	size;

	size = 1;
	if (cmds->flags)
		size++;
	size += len_list(cmds->args);
	return (size);
}

char	**cmd_to_argv(t_cmds *cmds, t_data *data)
{
	t_args	*iter;
	char	**argv;
	int		size;
	int		i;

	size = size_cmds(cmds);
	argv = ft_calloc(size + 1, sizeof(char *));
	if (!argv)
		cleanup(data, 2);
	argv[0] = ft_strdup(cmds->cmd);
	if (!argv[0])
	{
		free(argv);
		cleanup(data, 2);
	}
	i = copy_flags(data, argv, cmds);
	iter = cmds->args;
	while (iter)
	{
		argv[i++] = iter->arg;
		iter = iter->next;
	}
	return (argv);
}

void	free_argv(char **argv)
{
	int	i;

	i = -1;
	while (argv[++i] != 0)
		free(argv[i]);
}

/* Function that take the command and send it to find_path
 before executing it. */
int	execute(t_cmds *cmds, char **envp, t_sig **act, t_data *data)
{
	char	**argv;
	int		ran;

	argv = cmd_to_argv(cmds, data);
	ran = run(argv, envp, cmds, act);
	if (ran == -1 && access(cmds->cmd, F_OK) == -1)
	{
		free_argv(argv);
		free(argv);
		return (exec_no_file(cmds));
	}
	else if (ran == -1)
	{
		free_argv(argv);
		free(argv);
		return (exec_no_perm(cmds));
	}
	free(argv[0]);
	if (cmds->flags)
		free(argv[1]);
	free(argv);
	return (1);
}
