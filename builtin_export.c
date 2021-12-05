/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/05 18:30:42 by felipe            #+#    #+#             */
/*   Updated: 2021/12/05 19:57:59 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**ft_realloc(char ***array, int size)
{
	char	**new;
	int		prev_size;
	int		i;

	prev_size = 0;
	while ((*array)[prev_size] != 0)
		prev_size++;
	new = ft_calloc(prev_size + size, sizeof (char *));
	if (!new)
		return (0);
	i = -1;
	while (++i < prev_size)
		new[i] = (*array)[i];
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

int	builtin_export(t_cmds *cmds, t_vars *variables, char ***envp)
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
		size++;
		iter = iter->next;
	}
	prev_size = 0;
	while ((*envp)[prev_size] != 0)
		prev_size++;
	*envp = ft_realloc(envp, size);
	if (!(*envp))
		return (-1);
	iter = cmds->args;
	while (iter)
	{
		if (ft_strchr(iter->arg, '='))
			(*envp)[prev_size] = ft_strdup(iter->arg);
		else if (get_value(variables, iter->arg))
		{
			va = get_value(variables, iter->arg);
			temp = ft_strjoin(va->var, "=");
			(*envp)[prev_size] = ft_strjoin(temp, va->value);
			free(temp);
		}
		else
			(*envp)[prev_size] = ft_strjoin(iter->arg, "=");
		prev_size++;
		iter = iter->next;
	}
	return (0);
}