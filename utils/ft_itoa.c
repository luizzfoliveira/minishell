/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/10 20:37:32 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 20:43:01 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_num_len(int n)
{
	int				counter;
	unsigned int	n_uns;

	counter = 0;
	if (n < 0)
	{
		counter++;
		n_uns = -n;
	}
	else
		n_uns = n;
	while (n_uns / 10 != 0)
	{
		n_uns = n_uns / 10;
		counter++;
	}
	counter++;
	return (counter);
}

static int	ft_power(int n, int power)
{
	int	i;
	int	n_pow;

	i = 0;
	n_pow = 1;
	while (i < power)
	{
		n_pow = n_pow * n;
		i++;
	}
	return (n_pow);
}

char	*ft_itoa(int n)
{
	char			*nbr;
	int				n_len;
	int				i;
	unsigned int	n_uns;

	n_len = ft_num_len(n);
	nbr = malloc(n_len * sizeof (char) + 1);
	if (!nbr)
		return (0);
	i = -1;
	if (n < 0)
	{
		nbr[0] = '-';
		n_uns = -n;
		i++;
	}
	else
		n_uns = n;
	while (++i < n_len)
	{
		nbr[i] = n_uns / (ft_power(10, n_len - i - 1)) + '0';
		n_uns = n_uns % (ft_power(10, n_len - i - 1));
	}
	nbr[i] = 0;
	return (nbr);
}
