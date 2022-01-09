/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:17:13 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:17:14 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_cd(t_cmds *cmds, t_vars *variables)
{
	int	result;

	if (cmds->args->next)
	{
		printf("cd: too many arguments\n");
		g_reset_fd[2] = 1;
		return (1);
	}
	if (cmds->args->arg)
		result = chdir(cmds->args->arg);
	else
		result = chdir(get_variable("HOME", 4, variables));
	if (result == -1)
	{
		printf("cd: %s: No such file or directory\n", cmds->args->arg);
		g_reset_fd[2] = 1;
		return (1);
	}
	return (0);
}
