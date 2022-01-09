/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:45 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 11:18:06 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_args_utils(t_data *data, char *line, int i, t_args *iter)
{
	char	quote;
	int		j;

	quote = 0;
	j = 0;
	while ((line[i + j] != 0 && line[i + j] != '|' && line[i + j] \
	!= ';' && line[i + j] != '>' && !ft_isspace(line[i + j])) || quote)
	{
		if ((line[i + j] == '\'' || line[i + j] == '"') && !quote)
			quote = line[i + j];
		else if (line[i + j] == quote)
			quote = 0;
		j++;
	}
	iter->arg = ft_strndup(line + i, j);
	iter->next = 0;
	if (!iter->arg)
		cleanup(data, 2);
	return (j);
}

static int	get_args_utils2(t_data *data, char *line, t_args **iter, int i)
{
	int	j;

	j = 0;
	while (ft_isspace(line[i + j]))
		j++;
	if (line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
	{
		(*iter)->next = ft_calloc(1, sizeof (t_args));
		if (!(*iter)->next)
			cleanup(data, 2);
		(*iter) = (*iter)->next;
		(*iter)->arg = 0;
		(*iter)->next = 0;
	}
	return (j);
}

/* retorna os argumentos em uma lista */
t_args	*get_args(char *line, int *count, t_data *data)
{
	t_args	*args;
	t_args	*iter;
	int		i;

	args = ft_calloc(1, sizeof (t_args));
	if (!args)
		cleanup(data, 2);
	args->arg = 0;
	args->next = 0;
	iter = args;
	while (ft_isspace(*line))
	{
		(*count)++;
		line++;
	}
	i = 0;
	while (line[i] != 0 && line[i] != '|' && line[i] != ';'
		&& line[i] != '>' && line[i] != '<')
	{
		i += get_args_utils(data, line, i, iter);
		remove_char(iter->arg, get_quote(iter->arg));
		i += get_args_utils2(data, line, &iter, i);
	}
	(*count) += i;
	return (args);
}
