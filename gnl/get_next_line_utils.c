/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 17:26:16 by felipe            #+#    #+#             */
/*   Updated: 2021/11/03 14:29:10 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != 0)
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (s[i] != 0)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = s[i];
	return (dup);
}

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len] != 0)
		len++;
	return (len);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	if (c >= 256)
		c = c - 256;
	i = 0;
	while (s[i] != 0)
	{
		if (s[i] == c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == c)
		return ((char *)&s[i]);
	return (0);
}

char	*ft_concat(char **dest, char *src)
{
	char	*new;
	int		i;
	int		len_dest;
	int		len_src;

	len_dest = ft_strlen((*dest));
	len_src = ft_strlen(src);
	new = malloc(len_dest + len_src + 1);
	if (!new)
	{
		free((*dest));
		return (0);
	}
	i = -1;
	while (++i < len_dest)
		new[i] = (*dest)[i];
	while (i < len_dest + len_src)
	{
		new[i] = src[i - len_dest];
		i++;
	}
	new[i] = 0;
	free((*dest));
	return (new);
}
