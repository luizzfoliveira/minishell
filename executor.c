/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:19:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/05 19:59:30 by felipe           ###   ########.fr       */
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

void	executor(t_cmds *cmds, t_vars *variables, char ***envp)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;
	char	*line;

	next_arg = cmds->args;
	iter = cmds;
	while (iter != 0)
	{
		if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "env", ft_strlen(iter->cmd)))
			exec_env(*envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "echo", ft_strlen(iter->cmd)))
			ft_echo(iter);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "exit", ft_strlen(iter->cmd)))
			builtin_exit(iter, variables);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "export", ft_strlen(iter->cmd)))
			builtin_export(iter, variables, envp);
		else if (ft_strlen(iter->cmd) && !ft_strncmp(iter->cmd, "unset", ft_strlen(iter->cmd)))
			builtin_unset(iter, variables, envp);
		iter = iter->next;
	}
}
