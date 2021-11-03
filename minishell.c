/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/11/03 14:29:57 by felipe           ###   ########.fr       */
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

void	executor(int *cmds, int size, char **args)
{
	int	i;

	i = -1;
	while (++i < size)
	{
		if (cmds[i] == ECHO)
			printf("%s\n", args[i]);
		else if (cmds[i] == ECHO_OPT)
			printf("%s", args[i]);
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

/* char	*get_cmd(char *line, int *count)
{
	char	*cmd;
	int		i;

	i = 0;
	while (line[i] != 0 && line[i] != ' ' && line[i] != '|' && line[i] != ';')
		i++;
	if (line[0] == )
	cmd = ft_strndup(line, i);
	(*count) += i;
	return (cmd);
} */

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

/* fazer os argumentos em lista tambem? */
/* char	*get_args(char *line, int *count)
{
	char	*args;
	int		literal;
	int		i;

	literal = 0;
	i = 0;
	while (line[i] != 0 && line[i] != '|' && line[i] != ';')
		i++;
	if (line)
} */

/* ainda nao cuido de quando vem dois | juntos (||) */
/* t_cmds	*parser(char *line)
{
	t_cmds	*cmds;
	t_cmds	*iter;
	char	*quotes;
	int		i;
	int		j;

	quotes = ft_strchr(line, '"');
	cmds = malloc(sizeof (t_cmds));
	cmds->next = 0;
	iter = cmds;
	i = -1;
	j = -1;
	while (line[++j] != 0)
	{
		while (line[j] == ' ')
			j++;
		iter->cmd = get_cmd(line + j, &j);
		while (line[j] == ' ')
			j++;
		iter->flags = get_flags(line + j, &j);
		while (line[j] == ' ')
			j++;
		iter->arg = get_args(line + j, &j);
	}
	return (cmds);
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
	return ('"');
}

void	check_quotation(char **line)
{
	char	*quotes;
	char	*line_quotes;
	char	c;
	int		quote_count;

	c = get_quote(*line);
	quotes = ft_strchr(*line, &c);
	while (quotes)
	{
		quotes = ft_strchr(quotes + 1, &c);
		quote_count++;
	}
	while (quote_count % 2 != 0)
	{
		line_quotes = readline("> ");
		quotes = ft_strchr(line_quotes, &c);
		while (quotes)
		{
			quotes = ft_strchr(quotes + 1, &c);
			quote_count++;
		}
		*line = ft_concat(line, "\n");
		*line = ft_concat(line, line_quotes);
		free(line_quotes);
	}
} */

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
		/* check_quotation(&line); */
		write(log, line, ft_strlen(line));
		write(log, "\n", 1);
		/* cmds = parser(line); */
		/* if (!cmds)
		{
			printf("erro\n");
			return (0);
		} */
		/* executor(cmds); */
		free(line);
	}
	close(log);
}
