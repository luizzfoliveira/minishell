/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:17 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 11:38:43 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_flag_utils(char *line)
{
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (line[i] != 0 && !ft_isspace(line[i]) && \
	line[i] != '|' && line[i] != ';')
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	return (i);
}

// static int	check_echo()

/* retorna as flags como uma string */
char	*get_flags(char *line, int *count, t_data *data)
{
	char	*flags;
	char	quote;
	int		i;
	int		j;

	j = -1;
	while (ft_isspace(line[++j]))
		(*count)++;
	if (line[j] == '-' || ((line[j] == '"' || \
	line[j] == '\'') && line[j + 1] == '-'))
	{
		i = get_flag_utils(line + j);
		flags = ft_strndup(line + j, i);
		if (!flags)
			cleanup(data, 2);
		quote = get_quote(flags);
		remove_char(flags, quote);
		(*count) += i;
		return (flags);
	}
	return (0);
}
