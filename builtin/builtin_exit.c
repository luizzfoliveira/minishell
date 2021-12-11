/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/04 20:13:44 by felipe            #+#    #+#             */
/*   Updated: 2021/12/11 12:27:59 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_args(t_args *args)
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
}

int	exit_error(int err, char *arg)
{
	if (err == 1)
		printf("exit: too many arguments\n");
	else if (err == 2)
		printf("exit: %s: numeric argument required\n", arg);
	return (err);
}

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

int	builtin_exit(t_cmds *cmds, t_vars *variables)
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
	free_cmds(cmds);
	free_vars(variables);
	exit(err);
}
