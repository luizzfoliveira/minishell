/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:20:12 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 14:12:19 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cleanup_variables(t_data *data)
{
	t_vars	*iter;
	t_vars	*next;
	int		i;

	iter = data->variables;
	while (iter)
	{
		free(iter->var);
		free(iter->value);
		next = iter->next;
		free(iter);
		iter = next;
	}
	i = -1;
	while (data->envp[++i] != 0)
		free(data->envp[i]);
	free(data->envp);
}

void	free_args(t_args **args)
{
	t_args	*iter;
	t_args	*next;

	iter = *args;
	while (iter)
	{
		if (iter->arg)
			free(iter->arg);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	free_cmds(t_cmds **cmds)
{
	t_cmds	*iter;
	t_cmds	*next;

	iter = *cmds;
	while (iter)
	{
		if (iter->args)
			free_args(&iter->args);
		if (iter->flags)
		{
			free(iter->flags);
		}
		if (iter->cmd)
			free(iter->cmd);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	cleanup(t_data *data, int end)
{
	free_cmds(&data->cmds);
	if (end)
		cleanup_variables(data);
	if (end == 2)
	{
		rl_clear_history();
		exit(2);
	}
}
