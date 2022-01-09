/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:17:50 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:43:42 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	export_tvars(char *var, int change, char ***envp, t_data *data)
{
	t_vars	*va;
	char	*temp;

	free((*envp)[change]);
	va = get_value(data->variables, var);
	temp = ft_strjoin(va->var, "=");
	if (!temp)
		cleanup(data, 2);
	(*envp)[change] = ft_strjoin(temp, va->value);
	if (!(*envp)[change])
	{
		while (--change >= 0)
			free((*envp)[change]);
		cleanup(data, 2);
	}
	free(temp);
}

static void	change_env(char *var, t_vars **variables, char ***envp, \
t_data *data)
{
	int		change;
	int		i;

	change = find_env_var(var, *envp);
	if (ft_strchr(var, '='))
	{
		free((*envp)[change]);
		(*envp)[change] = ft_strdup(var);
		if (!(*envp)[change])
		{
			while (--change >= 0)
				free((*envp)[change]);
			cleanup(data, 2);
		}
		save_env_var(var, &i, data, 1);
	}
	else if (get_value(*variables, var))
		export_tvars(var, change, envp, data);
}

static void	init_sizes(int *size, int *prev_size, t_cmds *cmds, char ***envp)
{
	t_args	*iter;

	(*size) = 0;
	iter = cmds->args;
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			(*size)++;
		iter = iter->next;
	}
	*prev_size = 0;
	while ((*envp)[*prev_size] != 0)
		(*prev_size)++;
}

static void	new_env_var(char ***envp, int prev_size, t_data *data, t_args *iter)
{
	t_vars	*va;
	char	*temp;

	if (ft_strchr(iter->arg, '='))
	{
		(*envp)[prev_size] = ft_strdup(iter->arg);
		if (!(*envp)[prev_size])
			cleanup(data, 2);
		save_env_var(iter->arg, NULL, data, 1);
	}
	else if (get_value(data->variables, iter->arg))
	{
		va = get_value(data->variables, iter->arg);
		temp = ft_strjoin(va->var, "=");
		(*envp)[prev_size] = ft_strjoin(temp, va->value);
		free(temp);
		if (!(*envp)[prev_size])
			cleanup(data, 2);
	}
	else
	{
		(*envp)[prev_size] = ft_strjoin(iter->arg, "=");
		if (!(*envp)[prev_size])
			cleanup(data, 2);
	}
}

int	builtin_export(t_cmds *cmds, t_vars **variables, char ***envp, t_data *data)
{
	t_args	*iter;
	int		prev_size;
	int		size;

	iter = cmds->args;
	if (!(((char)iter->arg[0] >= 'a' && (char)iter->arg[0] <= 'z')
			|| ((char)iter->arg[0] >= 'A' && (char)iter->arg[0] <= 'Z')))
	{
		write(1, "minishell: not a valid identifier\n", 34);
		g_reset_fd[2] = 1;
		return (0);
	}
	init_sizes(&size, &prev_size, cmds, envp);
	ft_realloc(envp, size, data);
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			new_env_var(envp, prev_size, data, iter);
		else
			change_env(iter->arg, variables, envp, data);
		prev_size++;
		iter = iter->next;
	}
	return (0);
}
