/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:08 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 11:17:35 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_cmds(t_cmds *cmds)
{
	cmds->cmd = 0;
	cmds->flags = 0;
	cmds->args = 0;
	cmds->fd_in = 0;
	cmds->fd_out = 0;
	cmds->next = 0;
}

void	remove_char(char *s, char c)
{
	int	writer;
	int	reader;

	writer = 0;
	reader = 0;
	while (s[reader] != 0)
	{
		if (s[reader] != c)
			s[writer++] = s[reader];
		reader++;
	}
	s[writer] = 0;
}

char	get_quote(char *line)
{
	int	i;

	i = -1;
	while (line[++i] != 0)
	{
		if (line[i] == '"')
			return ('"');
		if (line[i] == '\'')
			return ('\'');
	}
	return (0);
}

char	*get_cmd(char *line, int *count, t_data *data)
{
	char	*cmd;
	char	quote;
	int		i;

	quote = 0;
	i = 0;
	while (ft_isspace(line[i]))
		i++;
	while ((line[i] != 0 && !ft_isspace(line[i])
			&& line[i] != '|' && line[i] != ';'
			&& line[i] != '<' && line[i] != '>') || quote)
	{
		if ((line[i] == '\'' || line[i] == '"') && !quote)
			quote = line[i];
		else if (line[i] == quote)
			quote = 0;
		i++;
	}
	cmd = ft_strndup(line, i);
	if (!cmd)
		cleanup(data, 2);
	quote = get_quote(cmd);
	remove_char(cmd, quote);
	(*count) += i;
	return (cmd);
}

char	*trunc_input_filename(char *line)
{
	char	*new_str;
	char	*newest_str;
	int		i;
	int		size;

	i = 1;
	size = 0;
	new_str = malloc(40 * sizeof(char));
	while (line[i] != 0 && ((line[i] >= 'a' && line[i] <= 'z') \
	|| (line[i] >= 'A' && line[i] <= 'Z') \
	|| (line[i] >= '0' && line[i] <= '9')))
	{
		new_str[size] = line[i];
		size++;
		i++;
	}
	new_str[++size] = '\0';
	newest_str = ft_strdup(new_str);
	free(new_str);
	return (newest_str);
}
