/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:14:28 by felipe            #+#    #+#             */
/*   Updated: 2021/12/05 19:12:31 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_variable(char *line, int size, t_vars *variables)
{
	t_vars	*iter;

	iter = variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, line, size))
			return (iter->value);
		iter = iter->next;
	}
	return ("");
}

char	*find_vars(char *line, char c)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (!quote && line[i] == '$')
			return (&line[i]);
		else if (line[i] == '\'' && !quote)
			quote = 1;
		else if (line[i] == '\'' && quote)
			quote = 0;
	}
	return (0);
}

void	substitute_variables(char **line, t_vars *variables)
{
	t_vars	*iter;
	char	*value;
	char	*vars;
	char	*temp;
	int		size;

	vars = find_vars(*line, '$');
	while (vars)
	{
		vars++;
		size = 0;
		while (vars[size] != ' ' && vars[size] != ';' && vars[size] != '|' && vars[size] != 0 && vars[size] != '"')
			size++;
		value = ft_strdup(get_variable(vars, size, variables));
		vars[-1] = 0;
		temp = ft_strdup(*line);
		temp = ft_concat(&temp, value);
		free(value);
		temp = ft_concat(&temp, vars + size);
		free(*line);
		*line = temp;
		vars = find_vars(*line, '$');
	}
}