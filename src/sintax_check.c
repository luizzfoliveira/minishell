/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sintax_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:19:57 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 21:59:55 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	sintax_check_aux(char *line, int i)
{
	i++;
	while (ft_isspace(line[i]))
		i++;
	if (line[i] == '|' || line[i] == '>' || line[i] == '<' || line[i] == '\0' )
	{
		if (!((line [i] == '>' && line [i - 1] == '>') \
		|| (line [i] == '<' && line [i - 1] == '<')))
		{
			while (ft_isspace(line[i]))
				i++;
			if (!((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' \
			&& line[i] <= 'Z') || (line[i] >= '0' && line[i] <= '9')))
				return (-1);
		}
	}
	else
	{
		while (ft_isspace(line[i]) || line[i] == '>' || line[i] == '<')
			i++;
		if (!((line[i] >= 'a' && line[i] <= 'z') || (line[i] >= 'A' \
		&& line[i] <= 'Z') || (line[i] >= '0' && line[i] <= '9')))
			return (-1);
	}
	return (0);
}

int	sintax_check(char *line)
{
	int	i;
	int	error;

	i = 0;
	error = 0;
	if (line[i] == '|')
		error = -1;
	while (line[i])
	{
		if (line[i] == '|' || line[i] == '>' || line[i] == '<')
			error = sintax_check_aux(line, i);
		if (error == -1)
			break ;
		i++;
	}
	if (error == -1)
		return (sintax_error());
	return (0);
}
