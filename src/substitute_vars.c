/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:20:04 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 11:52:11 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*find_vars(char *line)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (!quote && ((line[i] == '$' && (!ft_isspace(line[i + 1]) && \
		line[i + 1] != 0 && line[i + 1] != '|' && line[i + 1] != '<' \
		&& line[i + 1] != '>')) || (line[i] == '~' && (line[i + 1] == 0 \
		|| ft_isspace(line[i + 1])))))
			return (&line[i]);
		else if (line[i] == '\'' && !quote)
			quote = 1;
		else if (line[i] == '\'' && quote)
			quote = 0;
	}
	return (0);
}

void	change_pipe_result(char **line, int i)
{
	char	*result;
	char	*temp;

	result = ft_itoa(g_reset_fd[2]);
	(*line)[i] = 0;
	temp = ft_strdup(*line);
	temp = ft_concat(&temp, result);
	free(result);
	temp = ft_concat(&temp, *line + i + 2);
	free(*line);
	*line = temp;
}

static char	*copy_value(t_data *data, char **vars, int size)
{
	if ((*vars)[-1] == '~')
		return (ft_strdup(get_variable("HOME", 4, data->variables)));
	else
		return (ft_strdup(get_variable((*vars), size, data->variables)));
}

void	substitute_utils(char **vars, char **line, t_data *data)
{
	char	*temp;
	char	*value;
	int		size;

	(*vars)++;
	size = 0;
	while (!ft_isspace((*vars)[size]) && (*vars)[size] != ';' \
	&& (*vars)[size] != '|' && (*vars)[size] != 0 && (*vars)[size] != '"')
		size++;
	value = copy_value(data, vars, size);
	if (!value)
		cleanup(data, 2);
	(*vars)[-1] = 0;
	temp = ft_strdup(*line);
	if (!temp)
		cleanup(data, 2);
	temp = ft_concat(&temp, value);
	free(value);
	if (!temp)
		cleanup(data, 2);
	temp = ft_concat(&temp, (*vars) + size);
	free(*line);
	if (!temp)
		cleanup(data, 2);
	*line = temp;
}

void	substitute_variables(char **line, t_data *data)
{
	char	*vars;

	vars = find_vars(*line);
	while (vars)
	{
		if (vars[1] == '?')
		{
			change_pipe_result(line, vars - *line);
			vars++;
		}
		else
			substitute_utils(&vars, line, data);
		vars = find_vars(*line);
	}
}
