/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:10 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:18:11 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_env(char ***envp, t_args *iter)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while ((*envp)[++i] != 0)
	{
		if (!ft_strncmp(iter->arg, (*envp)[i], ft_strlen(iter->arg)) \
		&& (*envp)[i][ft_strlen(iter->arg)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = 0;
			j++;
		}
		(*envp)[i] = (*envp)[j];
		j++;
	}
}

void	unset_vars(t_args *iter, t_vars **variables)
{
	t_vars	*i_vars;
	t_vars	*prev;

	i_vars = *variables;
	while (i_vars)
	{
		if (!ft_strncmp(iter->arg, i_vars->var, ft_strlen(iter->arg)))
		{
			if (i_vars == *variables)
			{
				prev = *variables;
				*variables = (*variables)->next;
				free(prev);
				break ;
			}
			prev->next = i_vars->next;
			free(i_vars->var);
			free(i_vars->value);
			free(i_vars);
			break ;
		}
		prev = i_vars;
		i_vars = i_vars->next;
	}
}

int	builtin_unset(t_cmds *cmds, t_vars **variables, char ***envp)
{
	t_args	*iter;

	iter = cmds->args;
	while (iter)
	{
		unset_env(envp, iter);
		unset_vars(iter, variables);
		iter = iter->next;
	}
	return (0);
}
