/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:11:27 by felipe            #+#    #+#             */
/*   Updated: 2021/12/11 11:30:49 by felipe           ###   ########.fr       */
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

/* funcao para verificar se o comando passado é ou não
 * um comando aceito */
int	is_cmd(char *cmd)
{
	int	len;

	if (cmd)
	{
		len = ft_strlen(cmd);
		if (!ft_strncmp(cmd, "echo", len))
			return (1);
		else if (!ft_strncmp(cmd, "cd", len))
			return (1);
		else if (!ft_strncmp(cmd, "pwd", len))
			return (1);
		else if (!ft_strncmp(cmd, "export", len))
			return (1);
		else if (!ft_strncmp(cmd, "unset", len))
			return (1);
		else if (!ft_strncmp(cmd, "env", len))
			return (1);
		else if (!ft_strncmp(cmd, "exit", len))
			return (1);
		return (0);
	}
	return (1);
}

/* caso a flag não exista, essa função é chamada para
 * printar o erro no shell e dar free no que foi alocado */
int	flag_error(t_cmds *cmds)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;

	printf("%s: invalid option -- \'%s\'\n", cmds->cmd, &cmds->flags[1]);
	iter = cmds;
	while (iter != 0)
	{
		if (iter->flags)
			free(iter->flags);
		while (iter->args)
		{
			next_arg = iter->args->next;
			free(iter->args);
			iter->args = next_arg;
		}
		next = iter->next;
		free(iter);
		iter = next;
	}
	return (2);
}

/* caso o comando não exista, essa função é chamada para
 * printar o erro no shell e dar free no que foi alocado */
int	cmd_error(t_cmds *cmds)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;

	printf("%s: command not found\n", cmds->cmd);
	iter = cmds;
	while (iter != 0)
	{
		if (iter->flags)
			free(iter->flags);
		while (iter->args)
		{
			next_arg = iter->args->next;
			free(iter->args);
			iter->args = next_arg;
		}
		next = iter->next;
		free(iter);
		iter = next;
	}
	return (127);
}

/* funcao para verificar se os comandos e as flags existem */
int	check_cmds(t_cmds *cmds, char **envp)
{
	t_cmds	*iter;

	iter = cmds;
	while (iter)
	{
		if (!is_cmd(iter->cmd) && (iter->cmd[0] == '.' \
		|| iter->cmd[0] == '~' || iter->cmd[0] == '/' || find_path(iter->cmd, envp)))
			{}/* return (execute(iter, envp)); */
		else if (!is_cmd(iter->cmd))
			return (cmd_error(cmds));
		else if (!is_flag(iter))
			return (flag_error(cmds));
		iter = iter->next;
	}
	return (0);
}

/* verifica se há uma quantidade par da primeira aspas encontrada
 * ainda tenho que considerar o caso de encontrar aspas abertas e fechadas
 * de um jeito e depois aspas abertas de outro jeito */
int	check_quotation(char *line)
{
	char	quote;
	int		quote_counter;
	int		i;

	quote = 0;
	quote_counter = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (quote && line[i] == quote)
			quote = 0;
		else if (line[i] == '"')
			quote = '"';
		else if (line[i] == '\'')
			quote = '\'';
	}
	if (quote)
		return (-1);
	return (0);
}

int	check_unspecified_chars(char *line)
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
			printf("syntax error near unexpected token ';'\n");
			return (-1);
		}
	}
	return (0);
}
