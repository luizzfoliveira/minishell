/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirects.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:30 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 22:38:51 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	treat_input_utils(char *line, int i, t_cmds *cmds)
{
	char	*outfile;

	while (ft_isspace(line[i]))
		i++;
	while (!((line[i] >= 'a' && line[i] <= 'z') \
	|| (line[i] >= 'A' && line[i] <= 'Z') \
	|| (line[i] >= '0' && line[i] <= '9') \
	|| line[i] == '.'))
		i++;
	outfile = 0;
	outfile = ft_strword(line + i, outfile);
	cmds->fd_in = open_file(outfile, 2);
	if (cmds->fd_in == -1)
	{
		no_file(outfile);
		write(1, "\n", 1);
		g_reset_fd[2] = 127;
		free(outfile);
		return (1);
	}
	free(outfile);
	return (0);
}

int	red_routine(char *line, t_cmds *cmds)
{
	int	i;

	i = 0;
	while (line[i] && line[i] != '|')
	{
		if (line[i] == '<' || line[i] == '|')
		{
			if (line[i] == '|')
				return (0);
			if (treat_input_utils(line, i, cmds) == 1)
				return (1);
		}
		i++;
	}
	return (0);
}

char	*treat_input_red(char *line, t_cmds *cmds, t_sig **act)
{
	char	*limiter;
	int		i;

	i = 0;
	while (line[i] && line[i] != '|')
		i++;
	while (i >= 0 && line[i] != '<')
		i--;
	if (i > 0 && line[i - 1] == '<')
	{
		while (line[i] == '<' || ft_isspace(line[i]))
			i++;
		limiter = 0;
		limiter = ft_strword(line + i, limiter);
		i = here_doc(limiter, act, cmds);
		free(limiter);
		if (i != 0)
			return (0);
	}
	else
		if (red_routine(line, cmds) == 1)
			return (0);
	return (remove_input_char(&line[0]));
}

static void	get_redirect_utils(char *line, t_cmds *cmds, int o_file, int *count)
{
	char	*outfile;
	int		i;

	i = 0;
	while (line[i] == '>')
	{
		while (line[i] == '>' || ft_isspace(line[i]))
			i++;
		outfile = 0;
		outfile = ft_strword(line + i, outfile);
		cmds->fd_out = open_file(outfile, o_file);
		free(outfile);
		while ((line[i] >= 'a' && line[i] <= 'z') \
		|| (line[i] >= 'A' && line[i] <= 'Z') \
		|| (line[i] >= '0' && line[i] <= '9') \
		|| line[i] == '.')
			i++;
		while (ft_isspace(line[i]))
			(i)++;
	}
	(*count) += i;
}

void	get_redirect(char *line, int *count, t_cmds *cmds)
{
	while (ft_isspace(*line))
	{
		(*count)++;
		line++;
	}
	if (line[0] == '|' && line[1] != '|')
		cmds->fd_out = 1000;
	else if (line[0] == '>' && line[1] == '>' && line[2] != '>')
		get_redirect_utils(line, cmds, 0, count);
	else if (line[0] == '>' && line[1] != '>' && line[2] != '>')
		get_redirect_utils(line, cmds, 1, count);
	else
		cmds->fd_out = 1;
}
