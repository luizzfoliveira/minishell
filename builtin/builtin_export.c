/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 18:30:42 by felipe            #+#    #+#             */
/*   Updated: 2021/12/11 14:35:32 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char ***array, int size)
{
	char	**new;
	int		prev_size;
	int		i;

	prev_size = 0;
	while ((*array)[prev_size] != 0)
		prev_size++;
	new = ft_calloc(prev_size + size + 1, sizeof (char *));
	if (!new)
		return (0);
	i = -1;
	while (++i < prev_size)
	{
		new[i] = (*array)[i];
	}
	free(*array);
	*array = new;
	return (new);
}

t_vars	*get_value(t_vars *variables, char *var)
{
	t_vars	*iter;

	iter = variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, var, ft_strlen(var)))
			return (iter);
		iter = iter->next;
	}
	return (0);
	
}

int	find_env_var(char *var, char **envp)
{
	char	*equal;
	int		i;
	int		len;

	len = ft_strlen(var);
	equal = ft_strchr(var, '=');
	if (equal)
		len = equal - var;
	i = 0;
	while (envp[i])
	{
		if (!strncmp(var, envp[i], len) && \
		(envp[i][len] == '=' || envp[i][len] == 0))
			return (i);
		i++;
	}
	return (-1);
}

void	change_env(char *var, t_vars **variables, char ***envp)
{
	t_vars	*va;
	char	*temp;
	int		change;
	int		i;

	change = find_env_var(var, *envp);
	if (ft_strchr(var, '='))
	{
		free((*envp)[change]);
		(*envp)[change] = ft_strdup(var);
		save_env_var(var, &i, variables);
	}
	else if (get_value(*variables, var))
	{
		free((*envp)[change]);
		va = get_value(*variables, var);
		temp = ft_strjoin(va->var, "=");
		(*envp)[change] = ft_strjoin(temp, va->value);
		free(temp);
	}
	/* else
	{
		free(change);
		change = ft_strjoin(var, "=");
		save_env_var(change, &i, variables);
	} */
}

int	builtin_export(t_cmds *cmds, t_vars **variables, char ***envp)
{
	t_args	*iter;
	t_vars	*va;
	char	*temp;
	int		prev_size;
	int		size;
	int		i;

	size = 0;
	iter = cmds->args;
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
			size++;
		iter = iter->next;
	}
	prev_size = 0;
	while ((*envp)[prev_size] != 0)
		prev_size++;
	ft_realloc(envp, size);
	if (!(*envp))
		return (-1);
	iter = cmds->args;
	while (iter)
	{
		if (find_env_var(iter->arg, *envp) == -1)
		{
			if (ft_strchr(iter->arg, '='))
			{
				(*envp)[prev_size] = ft_strdup(iter->arg);
				save_env_var(iter->arg, &i, variables);
			}
			else if (get_value(*variables, iter->arg))
			{
				va = get_value(*variables, iter->arg);
				temp = ft_strjoin(va->var, "=");
				(*envp)[prev_size] = ft_strjoin(temp, va->value);
				free(temp);
			}
			else
				(*envp)[prev_size] = ft_strjoin(iter->arg, "=");
		}
		else
			change_env(iter->arg, variables, envp);
		prev_size++;
		iter = iter->next;
	}
	/* printf("\n\n\n");
	int k=0;
	while ((*envp)[k])
	{
		printf("envp k = %s\n", (*envp)[k]);
		k++;
	}
	printf("\n\n\n"); */
	return (0);
}
