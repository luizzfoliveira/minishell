/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/27 12:02:45 by felipe            #+#    #+#             */
/*   Updated: 2021/12/09 19:51:05 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	get_end_here(char **line, int pos)
{
	char	*end;
	char	*here;
	char	*text;
	int		fd[2];
	int		size;
	int		i;

	i = pos;
	while ((*line)[i] == ' ')
		i++;
	size = 0;
	while ((*line)[i + size] != 0 && (*line)[i + size] != ' ' && (*line)[i + size] != '|')
		size++;
	end = ft_strndup((*line) + i, size);
	size = i + size;
	while (pos - 2 < size)
	{
		(*line)[pos - 2] = ' ';
		pos++;
	}
	text = 0;
	while (1)
	{
		here = readline("> ");
		if (!ft_strncmp(here, end, ft_strlen(here)))
			break ;
		here = ft_concat(&here, "\n");
		if (!text)
			text = here;
		else
		{
			text = ft_concat(&text, here);
			free(here);
		}
	}
	free(here);
	pipe(fd);
	write(fd[1], text, ft_strlen(text));
	free(text);
	close(fd[1]);
	return (fd[0]);
}

int	get_input(char **line, int pos)
{
	char	*file_name;
	int		size;
	int		i;
	int		in_fd;

	i = pos;
	while ((*line)[i] == ' ')
		i++;
	size = 0;
	while ((*line)[i + size] != 0 && (*line)[i + size] != ' ' && (*line)[i + size] != '|')
		size++;
	file_name = ft_strndup((*line) + i, size);
	in_fd = open(file_name, O_RDONLY, 0777);
	size = i + size;
	while (pos - 1 < size)
	{
		(*line)[pos - 1] = ' ';
		pos++;
	}
	free(file_name);
	return (in_fd);
}

int	get_infile(char **line)
{
	char	*redirect;
	char	*last;
	int		infile;

	infile = 0;
	redirect = ft_strchr(*line, '<');
	while (redirect)
	{
		if (redirect[1] == '<' && redirect[2] == '<')
		{
			printf("syntax error near unexpected token '<'\n");
			return (-1);
		}
		else if (redirect[1] == '<' && redirect[2] != '<')
		{
			redirect++;
			infile = 2;
		}
		else if (redirect[1] != '<')
			infile = 1;
		last = redirect;
		redirect = ft_strchr(redirect + 1, '<');
	}
	if (infile == 1)
		infile = get_input(line, last - *line + 1);
	else if (infile == 2)
		infile = get_end_here(line, last - *line + 1);
	return (infile);
}

int	truncate_output(char **line, int pos)
{
	char	*file_name;
	int		size;
	int		i;
	int		out_fd;

	i = pos;
	while ((*line)[i] == ' ')
		i++;
	size = 0;
	while ((*line)[i + size] != 0 && (*line)[i + size] != ' ' && (*line)[i + size] != '|')
		size++;
	file_name = ft_strndup((*line) + i, size);
	out_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	size = i + size;
	while (pos - 1 < size)
	{
		(*line)[pos - 1] = ' ';
		pos++;
	}
	free(file_name);
	return (out_fd);
}

int	append_output(char **line, int pos)
{
	char	*file_name;
	int		size;
	int		i;
	int		out_fd;

	i = pos;
	while ((*line)[i] == ' ')
		i++;
	size = 0;
	while ((*line)[i + size] != 0 && (*line)[i + size] != ' ' && (*line)[i + size] != '|')
		size++;
	file_name = ft_strndup((*line) + i, size);
	out_fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0777);
	size = i + size;
	while (pos - 2 < size)
	{
		(*line)[pos - 2] = ' ';
		pos++;
	}
	free(file_name);
	return (out_fd);
}

int	get_outfile(char **line)
{
	char	*redirect;
	char	*last;
	int		outfile;

	outfile = STDOUT_FILENO;
	redirect = ft_strchr(*line, '>');
	while (redirect)
	{
		if (redirect[1] == '>' && redirect[2] == '>')
		{
			printf("syntax error near unexpected token '>'\n");
			return (-1);
		}
		else if (redirect[1] == '>' && redirect[2] != '<')
		{
			redirect++;
			outfile += 2;
		}
		else if (redirect[1] != '<')
			outfile += 1;
		last = redirect;
		redirect = ft_strchr(redirect + 1, '<');
	}
	if (outfile == 2)
		outfile = truncate_output(line, last - *line + 1);
	else if (outfile == 3)
		outfile = append_output(line, last - *line + 1);
	return (outfile);
}

/* funcao para verificar a presenca de ';' e iterar o parser
 * considerando que cada ';' é um fim de linha */
int	read_lines(char **line, t_vars **variables, char ***envp)
{
	t_data	data;
	int		i;
	int		infile;

	substitute_variables(line, *variables);
	data.heredoc = 0;
	data.file_in = get_infile(line);
	if (data.file_in < 0)
	{
		free(*line);
		return (1);
	}
	data.file_out = get_outfile(line);
	data.cmds = parser(*line, variables);
	if (!data.cmds)
	{
		printf("erro\n");
		return (0);
	}
	printf("file in = %d\n", data.file_in);
	printf("file out = %d\n", data.file_out);
	if (!check_cmds(data.cmds, *envp))
		executor(&data, *variables, envp);
	free(*line);
	return (1);
}

t_vars	*initialize_vars(t_vars **variables, char **envp)
{
	int	count;
	int	i;

	i = -1;
	while (envp[++i] != 0)
		save_env_var(envp[i], &count, variables);
}

/* essa funçao é o loop infinito pro programa ficar rodando
 * primeiro, lê do shell, passa pelo parser para gerar a lista
 * de comandos, checa se os comandos são aceitos e executa os
 * comandos */
int	main(int argc, char *argv[], char **envp)
{
	t_vars	*variables;
	t_cmds	*cmds;
	char	*line;
	char	*prompt;
	int		rd;
	int		i;

	initialize_vars(&variables, envp);
	recieve_signals();
	while (1)
	{
		prompt = get_prompt();
		line = readline(prompt);
		free(prompt);
		if (!line)
			break ;
		if (line[0] != 0)
		{
			add_history(line);
			if (check_quotation(line))
			{
				printf("Unclosed quotation\n");
				free(line);
			}
			else if (check_unspecified_chars(line))
				free(line);
			else if (!read_lines(&line, &variables, &envp))
				return (0);
		}
		else
			free(line);
	}
	rl_clear_history();
}
