/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:20:35 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/07 21:54:17 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c)
{
	if (c == ' ' || c == '\t' || c == '\v'
		|| c == '\n' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

static int	ft_isnum(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	is_neg;
	int	num;

	num = 0;
	is_neg = 0;
	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			is_neg = 1;
		i++;
	}
	if (!ft_isnum(str[i]))
		return (0);
	while (ft_isnum(str[i]))
	{
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (is_neg)
		return (-num);
	return (num);
}
