/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/14 15:49:39 by lbricio-          #+#    #+#             */
/*   Updated: 2022/01/06 22:11:23 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_strword(const char *s, char *dup)
{
	int		i;
	int		size;

	size = 0;
	while ((s[size] >= 'a' && s[size] <= 'z')
		|| (s[size] >= 'A' && s[size] <= 'Z')
		|| (s[size] >= '0' && s[size] <= '9')
		|| s[size] == '.')
		size++;
	dup = malloc(size + 1);
	if (!dup)
		return (0);
	i = 0;
	while (i < size)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	*ft_memset(void *b, int c, size_t len)
{
	while (len--)
		*((unsigned char *)b + len) = (unsigned char)c;
	return (b);
}

void	*ft_calloc(size_t count, size_t size)
{
	size_t	tot_size;
	void	*dst;

	tot_size = size * count;
	dst = malloc(tot_size);
	if (!(dst))
		return (0);
	ft_memset(dst, 0, tot_size);
	return (dst);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (n == 0)
		return (0);
	i = 0;
	while (s1[i] != 0 && s2[i] != 0 && i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i] || i == n - 1)
			break ;
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joined;
	int		len1;
	int		len2;
	int		i;

	len1 = 0;
	while (s1[len1] != 0)
		len1++;
	len2 = 0;
	while (s2[len2] != 0)
		len2++;
	joined = malloc(len1 + len2 + 1);
	if (!joined)
		return (0);
	i = 0;
	while (i < len1 || i < len2)
	{
		if (i < len1)
			joined[i] = s1[i];
		if (i < len2)
			joined[i + len1] = s2[i];
		i++;
	}
	joined[len2 + len1] = 0;
	return (joined);
}
