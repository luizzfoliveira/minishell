/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/11/19 20:11:51 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_env(t_vars *variables)
{
	t_vars	*iter;

	printf("%s=%s\n", variables->var, variables->value);
	/* iter = variables;
	while (iter)
	{
		printf("%s\n")
	} */
}

void	lstadd_back(t_vars **lst, t_vars *new)
{
	t_vars	*last;

	last = *lst;
	if (last == 0)
	{
		*lst = new;
		return ;
	}
	while (last->next != 0)
		last = last->next;
	last->next = new;
}

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

/* funcao para inicializar todos as variaveis da struct t_cmds */
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

void	executor(t_cmds *cmds, t_vars *variables)
{
	t_cmds	*iter;
	t_cmds	*next;
	t_args	*next_arg;

	iter = cmds;
	while (iter != 0)
	{
		/* printf("cmd = %s\n", iter->cmd); */
		if (!ft_strncmp(iter->cmd, "env", 3))
			exec_env(variables);
		else if (!ft_strncmp(iter->cmd, "echo", ft_strlen(iter->cmd)))
			ft_echo(iter);
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

/* funcao q duplica uma str s ate o tamanho len */
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

char	*get_variable(char *line, int size, t_vars *variables)
{
	t_vars	*iter;

	iter = variables;
	while (iter)
	{
		if (!ft_strncmp(iter->var, line, size))
			return (iter->value);
		iter = iter->next;
	}
	return ("");
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
	return (1);
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
	return (1);
}

void	execute(char *file_path, char **argv)
{
	pid_t	child;
	int		status;

	child = fork();
	if(child == 0)
	{
		execve(file_path, argv, NULL);
	}
	else
	{
		waitpid(child, &status, 0);
	}
}

void	execute_file(t_cmds *cmds)
{
	t_args	*iter;
	char	**argv;
	int		i;

	argv = malloc(sizeof (char *));
	i = 0;
	iter = cmds->args;
	while (iter)
	{
		argv[i] = iter->arg;
		i++;
		iter = iter->next;
	}
	if (access(cmds->cmd, F_OK) == -1)
		printf("%s: No such file or directory\n", cmds->cmd);
	else if (access(cmds->cmd, X_OK) == 0)
		execute(cmds->cmd, argv);
	else
		printf("%s: Permission denied\n", cmds->cmd);
}

/* funcao para verificar se os comandos e as flags existem */
int	check_cmds(t_cmds *cmds)
{
	t_cmds	*iter;

	iter = cmds;
	while (iter)
	{
		if (iter->cmd[0] == '.' || iter->cmd[0] == '~')
			execute_file(iter);
		if (!is_cmd(iter->cmd))
			return (cmd_error(cmds));
		if (!is_flag(iter))
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

/* usa a funcao getcwd para saber em qual diretorio estou
 * e retorna isso com um # no final para ser o prompt */
char	*get_prompt()
{
	char	path[500];
	char	*prompt;

	getcwd(path, 500);
	prompt = ft_strjoin(path, "# ");
	return (prompt);
}

char	*find_vars(char *line, char c)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (line[++i] != 0)
	{
		if (!quote && line[i] == '$')
			return (&line[i]);
		else if (line[i] == '\'' && !quote)
			quote = 1;
		else if (line[i] == '\'' && quote)
			quote = 0;
	}
}

void	substitute_variables(char **line, t_vars *variables)
{
	t_vars	*iter;
	char	*value;
	char	*vars;
	char	*temp;
	int		size;

	vars = find_vars(*line, '$');
	while (vars)
	{
		vars++;
		size = 0;
		while (vars[size] != ' ' && vars[size] != ';' && vars[size] != '|' && vars[size] != 0 && vars[size] != '"')
			size++;
		value = ft_strdup(get_variable(vars, size, variables));
		printf("value = %s\n", value);
		vars[-1] = 0;
		temp = ft_strdup(*line);
		temp = ft_concat(&temp, value);
		free(value);
		temp = ft_concat(&temp, vars + size);
		free(*line);
		*line = temp;
		vars = find_vars(*line, '$');
	}
}

/* funcao para verificar a presenca de ';' e iterar o parser
 * considerando que cada ';' é um fim de linha */
int	read_lines(char **line, t_vars **variables)
{
	t_cmds	*cmds;
	char	*sol;
	int		i;

	substitute_variables(line, *variables);
	sol = *line;
	while (sol != 0)
	{
		cmds = parser(sol, variables);
		if (!cmds)
		{
			printf("erro\n");
			return (0);
		}
		if (!check_cmds(cmds))
			executor(cmds, *variables);
		sol = ft_strchr(sol, ';');
		if (sol)
			sol++;
	}
	free(*line);
	return (1);
}

/* essa funçao é o loop infinito pro programa ficar rodando
 * primeiro, lê do shell, passa pelo parser para gerar a lista
 * de comandos, checa se os comandos são aceitos e executa os
 * comandos */
int	main(int argv, char *argc[], char **argenv)
{
	t_vars	*variables;
	t_cmds	*cmds;
	char	*line;
	char	*prompt;
	int		rd;
	int		i;

	variables = 0;
	recieve_signals();
	// start prompt
	while (1)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
			break ;
		add_history(line);
		if (check_quotation(line))
		{
			printf("Unclosed quotation\n");
			free(line);
		}
		else if (!read_lines(&line, &variables))
			return (0);
	}
	rl_clear_history();
}
