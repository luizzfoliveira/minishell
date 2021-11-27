/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/27 16:09:19 by felipe            #+#    #+#             */
/*   Updated: 2021/11/27 19:45:53 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* funcao para inicializar todos as variaveis da struct t_cmds */
void	init_cmds(t_cmds *cmds)
{
	cmds->cmd = 0;
	cmds->flags = 0;
	cmds->args = 0;
	cmds->out = 0;
	cmds->next = 0;
}

/* funcao para remover um char. Usada para remover as aspas */
void remove_char(char *s, char c)
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
	s[writer]=0;
}

/* retorna o primeiro tipo de aspas encontrada */
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

/* funçao para retornar um comando como string */
char	*get_cmd(char *line, int *count, t_vars *variables)
{
	char	*cmd;
	int		i;

	i = 0;
	while (line[i] != 0 && line[i] != ' ' && line[i] != '|' && line[i] != ';')
		i++;
	cmd = ft_strndup(line, i);
	(*count) += i;
	return (cmd);
}

/* retorna as flags como uma string */
char	*get_flags(char *line, int *count)
{
	char	*flags;
	int		i;

	if (line[0] == '-')
	{
		i = 0;
		while (line[i] != 0 && line[i] != ' ' && line[i] != '|' && line[i] != ';')
			i++;
		flags = ft_strndup(line, i);
		(*count) += i;
		return (flags);
	}
	return (0);
}

/* retorna os argumentos em uma lista */
t_args	*get_args(char *line, int *count)
{
	t_args	*args;
	t_args	*iter;
	char	quote;
	int		quote_count;
	int		i;
	int		j;

	args = malloc(sizeof (t_args));
	args->arg = 0;
	args->next = 0;
	iter = args;
	quote_count = 0;
	i = 0;
	while (line[i] != 0 && line[i] != '|' && line[i] != ';')
	{
		quote = get_quote(line + i);
		j = 0;
		while (line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
		{
			if (line[i + j] == quote)
				quote_count++;
			if ((quote_count == 2 || !quote) && line[i + j] == ' ')
				break ;
			j++;
		}
		iter->arg = ft_strndup(line + i, j);
		iter->next = 0;
		if (quote)
			remove_char(iter->arg, quote);
		while (line[i + j] == ' ')
			j++;
		if (line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
		{
			iter->next = malloc(sizeof (t_args));
			iter = iter->next;
			iter->next = 0;
		}
		i += j;
	}
	(*count) += i;
	return (args);
}

void	add_variable(t_vars **variables, t_vars *new)
{
	t_vars	*iter;
	int		size_new;

	size_new = ft_strlen(new->var);
	iter = *variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, new->var, size_new))
		{
			free(iter->value);
			iter->value = new->value;
			free(new->var);
			return ;
		}
		iter = iter->next;
	}
	lstadd_back(variables, new);
}

void	save_env_var(char *line, int *count, t_vars **variables)
{
	t_vars	*new;
	char	quote;
	int		quote_count;
	int		equal;
	int		end;
	int		i;

	if (line[0] == '"' || line[0] == '\'')
		return ;
	end = 0;
	equal = 0;
	quote = get_quote(line);
	quote_count = 0;
	i = -1;
	while (line[++i] != 0 && line[i] != ';' && line[i] != '|')
	{
		if (line[i] == '=')
			equal = i;
		else if (line[i] == ' ' && equal == 0)
			return ;
		else if (line[i] == ' ' && (!quote || quote_count == 2))
			break ;
		else if (line[i] == quote)
			quote_count++;
	}
	end = i;
	while (line[i] == ' ')
		i++;
	if (equal)
		(*count) += end;
	if (line[i] != 0 && line[i] != ';')
		return ;
	if (equal)
	{
		new = malloc(sizeof (t_vars));
		new->value = ft_strndup(line + equal + 1, end - equal - 1);
		remove_char(new->value, '"');
		remove_char(new->value, '\'');
		new->var = ft_strndup(line, equal);
		new->next = 0;
		add_variable(variables, new);
		/* t_vars *iter;
		iter = *variables;
		while (iter)
		{
			printf("var = %s\n", iter->var);
			printf("value = %s\n", iter->value);
			iter = iter->next;
		} */
	}
}

/* funçao que lê os caracteres da linha e cria a struct de comandos.
 * Esta funcao ignora os espaços em branco. Ela itera por todos os
 * caracteres da linha e retorna a primeira palavra encontrada como
 * sendo o comando, se a segunda palavra tiver um '-' retorna isso
 * como flag. Retorna o que sobrou como uma lista de argumentos */
t_cmds	*parser(char *line, t_vars **variables)
{
	t_cmds	*cmds;
	t_cmds	*iter;
	int		j;

	cmds = malloc(sizeof (t_cmds));
	init_cmds(cmds);
	iter = cmds;
	j = 0;
	while (line[j] != 0 && line[j] != ';')
	{
		while (line[j] == ' ')
			j++;
		save_env_var(line + j, &j, variables);
		while (line[j] == ' ')
			j++;
		iter->cmd = get_cmd(line + j, &j, *variables);
		remove_char(iter->cmd, get_quote(iter->cmd));
		while (line[j] == ' ')
			j++;
		iter->flags = get_flags(line + j, &j);
		while (line[j] == ' ')
			j++;
		iter->args = get_args(line + j, &j);
		while (line[j] == ' ')
			j++;
		if (line[j] == '|' && line[j + 1] != '|')
		{
			iter->next = malloc(sizeof (t_cmds));
			iter = iter->next;
			init_cmds(iter);
			j++;
		}
	}
	return (cmds);
}