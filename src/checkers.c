/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:15 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 22:08:02 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* funcao para verificar se a flag passada é valida para o comando
 * passado */

int	is_flag(t_cmds *cmds)
{
	if (cmds->flags)
	{
		if (!ft_strncmp(cmds->cmd, "echo", ft_strlen(cmds->cmd)) \
		&& !ft_strncmp(cmds->flags, "-n", ft_strlen(cmds->flags)))
			return (1);
		return (0);
	}
	return (1);
}

/* funcao para verificar se os comandos e as flags existem */
int	check_cmds(t_cmds *cmds, char **envp)
{
	char	*path;

	if (!cmds)
		return (0);
	path = find_path(cmds->cmd, envp);
	if (!is_builtin(cmds->cmd) && (cmds->cmd[0] == '.' \
	|| cmds->cmd[0] == '~' || cmds->cmd[0] == '/' || path))
	{
		if (access(cmds->cmd, F_OK) == -1 && (cmds->cmd[0] == '.' || \
		cmds->cmd[0] == '/' || cmds->cmd[0] == '~' || access(path, F_OK) == -1))
		{
			free(path);
			return (file_error(cmds));
		}
		free(path);
	}
	else
	{
		free(path);
		if (!is_builtin(cmds->cmd))
			return (cmd_error(cmds));
		else if (!is_flag(cmds))
			return (flag_error(cmds));
	}
	return (0);
}

/* verifica se há uma quantidade par da primeira aspas encontrada
 * ainda tenho que considerar o caso de encontrar aspas abertas e fechadas
 * de um jeito e depois aspas abertas de outro jeito */
int	check_quotation(char *line, t_data *data)
{
	char	quote;
	int		i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (quote && line[i] == quote)
			quote = 0;
		else if (line[i] == '"' && !quote)
			quote = '"';
		else if (line[i] == '\'' && !quote)
			quote = '\'';
	}
	if (quote)
	{
		cleanup(data, 0);
		return (-1);
	}
	return (0);
}

int	check_unspecified_chars(char *line, t_data *data)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (line[i] == '"' && !quote)
			quote = '"';
		else if (line[i] == '\'' && !quote)
			quote = '\'';
		else if (line[i] == quote)
			quote = 0;
		else if (!quote && (line[i] == ';' || line[i] == '\\'))
		{
			cleanup(data, 0);
			printf("syntax error near unexpected token\n");
			return (-1);
		}
	}
	return (0);
}
