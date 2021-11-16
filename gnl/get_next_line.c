/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: felipe <felipe@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/26 17:26:23 by felipe            #+#    #+#             */
/*   Updated: 2021/11/09 21:52:31 by felipe           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_linecopy(char **line, char *src)
{
	char	*temp;
	int		i;

	i = 0;
	while (src[i] != '\n' && src[i] != 0)
		i++;
	if (src[i] == '\n')
		temp = ft_strdup(&src[i + 1]);
	else
		temp = 0;
	src[i] = 0;
	*line = ft_strdup(src);
	if (!(*line))
		return (0);
	free(src);
	return (temp);
}

int	ft_eof(char **line, char **temp)
{
	free(*temp);
	*temp = 0;
	*line = ft_strdup("");
	return (0);
}

int	ft_output(char **line, char **temp, int rd)
{
	if (rd < 0)
		return (-1);
	else if (rd == 0 && *temp == 0)
	{
		free(*temp);
		*temp = 0;
		*line = ft_strdup("");
		return (0);
	}
	else
		*temp = ft_linecopy(line, *temp);
	if (*temp == 0)
		return (0);
	return (1);
}

int	get_next_line(int fd, char **line)
{
	static char	*temp[FD_MAX];
	char		*c;
	int			rd;

	if (fd < 0 || fd > FD_MAX || !line || BUFFER_SIZE <= 0)
		return (-1);
	c = malloc(BUFFER_SIZE + 1);
	if (!c)
		return (-1);
	rd = read(fd, c, BUFFER_SIZE);
	while (rd > 0)
	{
		c[rd] = 0;
		if (!temp[fd])
			temp[fd] = ft_strdup(c);
		else
			temp[fd] = ft_concat(&temp[fd], c);
		if (ft_strchr(temp[fd], '\n') != 0)
			break ;
		rd = read(fd, c, BUFFER_SIZE);
	}
	free(c);
	return (ft_output(line, &temp[fd], rd));
}
