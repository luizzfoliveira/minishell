/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:01 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 13:40:28 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	g_reset_fd[3];

int	read_lines(char **line, t_data *data, char ***envp, t_sig **act)
{
	t_cmds	*last;
	int		err;

	substitute_variables(line, data);
	err = parser(*line, data, envp, act);
	last = data->cmds;
	while (last->next)
	{
		if (last->fd_in != 0)
			break ;
		last = last->next;
	}
	if (last != data->cmds && data->cmds->fd_in == 0 \
	&& last->fd_in != 0 && last->next == 0)
	{
		data->cmds->fd_in = last->fd_in;
		last->fd_in = 0;
	}
	free(*line);
	return (err);
}

void	initialize_vars(t_data *data, char **envp)
{
	int	count;
	int	i;

	save_origin_fd();
	count = 0;
	i = -1;
	while (envp[++i] != 0)
		save_env_var(envp[i], &count, data, 1);
}

char	**copy_envp(char **envp)
{
	char	**new;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	new = ft_calloc(i + 1, sizeof (char *));
	if (!new)
		return (0);
	i = -1;
	while (envp[++i])
	{
		new[i] = ft_strdup(envp[i]);
		if (!new[i])
		{
			while (--i > 0)
				free(new[i]);
			return (0);
		}
	}
	return (new);
}

int	run_parser(char *line, t_data *data, t_sig **act)
{
	if (!line)
	{
		data->cmds = 0;
		cleanup(data, 2);
	}
	if (line[0] != 0)
	{
		add_history(line);
		if (check_quotation(line, data))
		{
			printf("Unclosed quotation\n");
			free(line);
		}
		else if (check_unspecified_chars(line, data))
			free(line);
		else if (read_lines(&line, data, &data->envp, act) == -1)
			return (-1);
	}
	else
		free(line);
	return (0);
}

int	main(int argc, char *argv[], char **envp)
{
	t_data	data;
	char	*line;
	t_sig	act;
	t_sig	act_quit;

	data.variables = 0;
	data.envp = copy_envp(envp);
	if (argc != 1 || argv[1])
		return (0);
	initialize_vars(&data, envp);
	config_sigaction(&act, sigint_handle, SIGINT);
	config_sigaction(&act_quit, SIG_IGN, SIGQUIT);
	while (1)
	{
		data.cmds = 0;
		line = readline("\001\033[1;33m\002Minishell> \001\033[0m\002");
		if (run_parser(line, &data, (void *)&act) != -1)
			executor(&data, &data.envp, (void *)&act);
		cleanup(&data, 0);
	}
	rl_clear_history();
}
