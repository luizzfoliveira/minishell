/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substitute_vars_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 22:42:56 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 22:43:08 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
