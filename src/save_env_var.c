/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_env_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:40 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 21:59:16 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	add_variable(t_data *data, t_vars *new)
{
	t_vars	*iter;
	int		size_new;

	size_new = ft_strlen(new->var);
	iter = data->variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, new->var, size_new) \
		&& size_new == ft_strlen(iter->var))
		{
			free(iter->value);
			iter->value = new->value;
			free(new->var);
			free(new);
			return ;
		}
		iter = iter->next;
	}
	lstadd_back(data, new);
}

static int	get_end_var(char *line, int env, int *end, int *equal)
{
	char	quote;
	int		quote_count;
	int		i;

	*equal = 0;
	quote = get_quote(line);
	quote_count = 0;
	i = -1;
	while (line[++i] != 0 && ((line[i] != ';' && line[i] != '|') || env))
	{
		if (line[i] == '=')
			*equal = i;
		else if (ft_isspace(line[i]) && *equal == 0)
			return (-1);
		else if (ft_isspace(line[i]) && (!quote || quote_count == 2) && !env)
			break ;
		else if (line[i] == quote)
			quote_count++;
	}
	*end = i;
	return (i);
}

static void	create_new_var(t_data *data, char *line, int end, int equal)
{
	t_vars	*new;

	new = ft_calloc(1, sizeof (t_vars));
	if (!new)
		cleanup(data, 2);
	new->value = ft_strndup(line + equal + 1, end - equal - 1);
	if (!new->value)
		cleanup(data, 2);
	remove_char(new->value, '"');
	remove_char(new->value, '\'');
	new->var = ft_strndup(line, equal);
	if (!new->var)
		cleanup(data, 2);
	new->next = 0;
	add_variable(data, new);
}

void	save_env_var(char *line, int *count, t_data *data, int env)
{
	int		equal;
	int		end;
	int		i;

	if (line[0] == '"' || line[0] == '\'')
		return ;
	i = get_end_var(line, env, &end, &equal);
	if (i == -1)
		return ;
	while (ft_isspace(line[i]))
		i++;
	if (equal && count)
		(*count) += end;
	if (line[i] != 0 && line[i] != ';')
		return ;
	if (equal)
		create_new_var(data, line, end, equal);
}
