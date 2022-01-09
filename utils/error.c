/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/13 12:12:06 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/07 19:09:45 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	no_file(char *file)
{
	write(2, "minishell: ", 11);
	write(2, file, ft_strlen(file));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	return (127);
}

int	sintax_error(void)
{
	write(1, "sintax error near unexpected token", 35);
	write(1, "\n", 1);
	g_reset_fd[2] = 2;
	return (-1);
}

int	flag_error(t_cmds *cmds)
{
	t_args	*new;

	if (!ft_strncmp(cmds->cmd, "echo", 4) && cmds->cmd[4] == 0)
	{
		new = malloc(sizeof (t_args));
		new->arg = cmds->flags;
		new->next = cmds->args;
		cmds->args = new;
		cmds->flags = 0;
		return (0);
	}
	printf("%s: invalid option -- \'%s\'\n", cmds->cmd, &cmds->flags[1]);
	g_reset_fd[2] = 2;
	return (1);
}

int	cmd_error(t_cmds *cmds)
{
	printf("%s: command not found\n", cmds->cmd);
	g_reset_fd[2] = 127;
	return (1);
}

int	file_error(t_cmds *cmds)
{
	printf("%s: No such file or directory\n", cmds->cmd);
	g_reset_fd[2] = 127;
	return (1);
}
