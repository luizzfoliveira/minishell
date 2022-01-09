/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:17:41 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:17:42 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_realloc(char ***array, int size, t_data *data)
{
	char	**new;
	int		prev_size;
	int		i;

	prev_size = 0;
	while ((*array)[prev_size] != 0)
		prev_size++;
	new = ft_calloc(prev_size + size + 1, sizeof (char *));
	if (!new)
		cleanup(data, 2);
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
