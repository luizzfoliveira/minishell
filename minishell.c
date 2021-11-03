/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/11/03 20:16:56 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_history(int log)
{
	char	*line;
	int		rd;
	int		i;

	i = -1;
	rd = 1;
	while (rd > 0)
	{
		rd = get_next_line(log, &line);
		printf("rd = %d\n", rd);
		printf("%d %s\n", ++i, line);
		free(line);
	}
}

void	init_cmds(t_cmds *cmds)
{
	cmds->cmd = 0;
	cmds->flags = 0;
	cmds->args = 0;
	cmds->out = 0;
	cmds->next = 0;
}

/* manage error */
int	str_to_cmd(char *str, int *j)
{
	int	len;
	int	cmd;

	cmd = -1;
	len = 0;
	while (str[len + *j] != ' ' && str[len + *j] != '|' && str[len + *j] != 0)
		len++;
	if (!ft_strncmp(str + *j, "echo", len))
		cmd = ECHO;
	else if (!ft_strncmp(str + *j, "cd", len))
		cmd = CD;
	else if (!ft_strncmp(str + *j, "pwd", len))
		cmd = PWD;
	else if (!ft_strncmp(str + *j, "export", len))
		cmd = EXPORT;
	else if (!ft_strncmp(str + *j, "unset", len))
		cmd = UNSET;
	else if (!ft_strncmp(str + *j, "env", len))
		cmd = ENV;
	else if (!ft_strncmp(str + *j, "exit", len))
		cmd = EXIT;
	(*j) += len;
	return (cmd);
}

void	executor(t_cmds *cmds)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;

	iter = cmds;
	while (iter != 0)
	{
		printf("cmd = %s\n", iter->cmd);
		if (iter->flags)
		{
			printf("flags = %s\n", iter->flags);
			free(iter->flags);
		}
		while (iter->args)
		{
			printf("args = %s\n", iter->args->arg);
			next_arg = iter->args->next;
			free(iter->args);
			iter->args = next_arg;
		}
		next = iter->next;
		free(iter);
		iter = next;
	}
}

char	*ft_strndup(const char *s, int len)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != 0 && i < len)
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (s[i] != 0 && i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

void	get_arg(char *line, int *j, char **arg)
{
	char	*buff;
	int		len;

	len = 0;
	while (line[*j + len] != '|' && line[*j + len] != 0)
		len++;
	if (len != 0)
	{
		buff = ft_strndup(line + *j, len);
		(*j) += len;
		*arg = buff;
	}
	else
	{
		buff = ft_strndup("", 1);
		*arg = buff;
	}
}

char	*get_cmd(char *line, int *count)
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

/* fazer os argumentos em lista tambem? */
t_args	*get_args(char *line, int *count)
{
	t_args	*args;
	t_args	*iter;
	char	quote;
	int		i;
	int		j;

	quote = get_quote(line);
	args = malloc(sizeof (t_args));
	args->arg = 0;
	args->next = 0;
	iter = args;
	i = 0;
	while (line[i] != 0 && line[i] != '|' && line[i] != ';')
	{
		j = 0;
		if (quote)
			while (line[i + j] != quote)
				j++;
		else
			while (line[i + j] != ' ' && line[i + j] != 0 && line[i + j] != '|' && line[i + j] != ';')
				j++;
		iter->arg = ft_strndup(line + i, j);
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

/* ainda nao cuido de quando vem dois | juntos (||)
 * também preciso cuidar de quando passo o ; na linha */
t_cmds	*parser(char *line)
{
	t_cmds	*cmds;
	t_cmds	*iter;
	char	*quotes;
	int		i;
	int		j;

	quotes = ft_strchr(line, '"');
	cmds = malloc(sizeof (t_cmds));
	init_cmds(cmds);
	iter = cmds;
	i = -1;
	j = 0;
	while (line[j] != 0)
	{
		while (line[j] == ' ')
			j++;
		iter->cmd = get_cmd(line + j, &j);
		while (line[j] == ' ')
			j++;
		iter->flags = get_flags(line + j, &j);
		while (line[j] == ' ')
			j++;
		iter->args = get_args(line + j, &j);
		while (line[j] == ' ')
			j++;
		if (line[j] == '|')
		{
			iter->next = malloc(sizeof (t_cmds));
			iter = iter->next;
			init_cmds(iter);
			j++;
		}
	}
	return (cmds);
}

void	check_quotation(char **line)
{
	char	*quotes;
	char	*line_quotes;
	char	c;
	int		quote_count;

	quote_count = 0;
	c = get_quote(*line);
	if (!c)
		return ;
	quotes = ft_strchr(*line, c);
	while (quotes)
	{
		quotes = ft_strchr(quotes + 1, c);
		quote_count++;
	}
	while (quote_count % 2 != 0)
	{
		line_quotes = readline("> ");
		quotes = ft_strchr(line_quotes, c);
		while (quotes)
		{
			quotes = ft_strchr(quotes + 1, c);
			quote_count++;
		}
		*line = ft_concat(line, "\n");
		*line = ft_concat(line, line_quotes);
		free(line_quotes);
	}
}

/* ctrl d ta dando errado depois do ctrl c. Tem q corrigir o ctrl c */
int	main(void)
{
	t_cmds	*cmds;
	char	*quotes;
	char	*line;
	int		quote_count;
	int		log;
	int		i;

	log = open("minishell.log", O_CREAT | O_RDWR);
	// start prompt
	while (1)
	{
		recieve_signals();
		line = readline("# ");
		if (!line)
			break ;
		check_quotation(&line);
		write(log, line, ft_strlen(line));
		write(log, "\n", 1);
		cmds = parser(line);
		if (!cmds)
		{
			printf("erro\n");
			return (0);
		}
		executor(cmds);
		free(line);
	}
	close(log);
}
