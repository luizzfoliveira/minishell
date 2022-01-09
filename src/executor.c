/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:39 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 20:01:40 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, t_sig **act)
{
	t_cmds	*iter;

	iter = cmds;
	if (iter != 0)
	{
		if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", 3))
			builtin_red(cmds, act, 3, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "echo", 4))
			builtin_red(cmds, act, 1, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "pwd", 3))
			builtin_red(cmds, act, 2, *envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "exit", 4))
			builtin_exit(iter, data);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "export", 6))
			builtin_export(iter, &data->variables, envp, data);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "unset", 5))
			builtin_unset(iter, &data->variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "cd", 2))
			builtin_cd(iter, data->variables);
	}
}

void	executor(t_data *data, char ***envp, t_sig **act)
{
	t_cmds	*iter;
	char	*path;

	iter = data->cmds;
	while (iter)
	{
		path = find_path(iter->cmd, *envp);
		if (!is_builtin(iter->cmd) && (iter->cmd[0] == '.' \
		|| iter->cmd[0] == '~' || iter->cmd[0] == '/' || path) \
		&& iter->cmd[0] != 0)
		{
			g_reset_fd[2] = 0;
			execute(iter, *envp, act, data);
		}
		else if (iter->cmd[0] != 0)
			exec_builtin(iter, data, envp, act);
		iter = iter->next;
		free(path);
	}
	reset_input();
	reset_output();
}

int	check_builtin_name(char *cmd, char *name)
{
	if (ft_strlen(cmd) == ft_strlen(name))
	{
		if (!(strncmp(cmd, name, ft_strlen(cmd))))
			return (1);
	}
	return (0);
}

int	is_builtin(char *cmd)
{
	if (cmd)
	{
		if (check_builtin_name(cmd, "echo"))
			return (1);
		else if (check_builtin_name(cmd, "cd"))
			return (1);
		else if (check_builtin_name(cmd, "pwd"))
			return (1);
		else if (check_builtin_name(cmd, "export"))
			return (1);
		else if (check_builtin_name(cmd, "unset"))
			return (1);
		else if (check_builtin_name(cmd, "env"))
			return (1);
		else if (check_builtin_name(cmd, "exit"))
			return (1);
		return (0);
	}
	return (1);
}
