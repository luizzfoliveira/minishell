/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:17:29 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:17:32 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/* void	free_args(t_args *args)
{
	t_args	*iter;
	t_args	*next;

	iter = args;
	while (iter)
	{
		free(iter->arg);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	free_cmds(t_cmds *cmds)
{
	t_cmds	*iter;
	t_cmds	*next;

	iter = cmds;
	while (iter)
	{
		free_args(iter->args);
		free(iter->cmd);
		free(iter->flags);
		next = iter->next;
		free(iter);
		iter = next;
	}
}

void	free_vars(t_vars *variables)
{
	t_vars	*iter;
	t_vars	*next;

	iter = variables;
	while (iter)
	{
		free(iter->var);
		free(iter->value);
		next = iter->next;
		free(iter);
		iter = next;
	}
} */

static int	check_arg(char *arg)
{
	int	i;

	i = -1;
	while (arg[++i] != 0)
	{
		if (arg[i] < '0' || arg[i] > '9')
			return (2);
	}
	return (0);
}

int	builtin_exit(t_cmds *cmds, t_data *data)
{
	unsigned char	err;
	t_args			*iter;
	int				size;

	size = 0;
	iter = cmds->args;
	while (iter && iter->arg)
	{
		size++;
		iter = iter->next;
	}
	if (cmds->args->arg && check_arg(cmds->args->arg))
		err = exit_error(2, cmds->args->arg);
	else if (size > 1)
		return (exit_error(1, 0));
	else if (size == 1)
		err = (unsigned char)ft_atoi(cmds->args->arg);
	else
		err = 0;
	cleanup(data, 1);
	exit(err);
}
