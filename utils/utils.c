/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbricio- <lbricio-@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 21:17:18 by felipe            #+#    #+#             */
/*   Updated: 2021/12/10 01:50:35 by lbricio-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	lstadd_back(t_vars **lst, t_vars *new)
{
	t_vars	*last;

	last = *lst;
	if (last == 0)
	{
		*lst = new;
		return ;
	}
	while (last->next != 0)
		last = last->next;
	last->next = new;
}

/* funcao q duplica uma str s ate o tamanho len */
char	*ft_strndup(const char *s, int len)
{
	char	*dup;
	int		i;

	i = 0;
	while (s[i] != 0 && i < len)
		i++;
	dup = malloc(i + 1);
	if (!dup)
		return (0);
	i = 0;
	while (s[i] != 0 && i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

/* duplica uma string até o fim de uma palavra, s[0] deve ser um caractere*/
char	*ft_strword(const char *s)
{
	char	*dup;
	int		i;
	int		u;

	i = 0;
	while (s[i] != 0)
		while (s[i] >= 'a' && s[i] <= 'z' 
		|| s[i] >= 'A' && s[i] <= 'Z' 
		||  s[i] >= '0' && s[i] <= '9')
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
	dup[i] = 0;
	return (dup);
}


void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*alloced;

	if (nmemb == 0 || size == 0)
	{
		alloced = malloc(1);
		*alloced = 0;
		return (alloced);
	}
	alloced = malloc(nmemb * size);
	if (!alloced)
		return (0);
	i = 0;
	while (i < nmemb * size)
	{
		alloced[i] = 0;
		i++;
	}
	return (alloced);
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

char	*ft_strnstr(const char	*big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (little[i] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while (big[i + j] == little[j] && i + j < len)
		{
			if (little[j + 1] == '\0')
				return ((char *)big + i);
			j++;
		}
		i++;
	}
	return (0);
}

char	*cmds_to_string(t_cmds *cmds)
{
	t_args	*iter;
	char	*line;
	char	*temp;

	line = ft_strdup(cmds->cmd);
	if (cmds->flags)
	{
		temp = ft_strjoin(line, " ");
		free(line);
		line = ft_strjoin(temp, cmds->flags);
		free(temp);
	}
	iter = cmds->args;
	while (iter && iter->arg != 0)
	{
		temp = ft_strjoin(line, " ");
		free(line);
		line = ft_strjoin(temp, iter->arg);
		free(temp);
	}
	return (line);
}
