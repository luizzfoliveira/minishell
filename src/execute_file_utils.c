/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:18:21 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/03 11:18:22 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	len_list(t_args *list)
{
	t_args	*iter;
	int		i;

	i = 0;
	iter = list;
	while (iter)
	{
		i++;
		iter = iter->next;
	}
	return (i);
}

void	free_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i] != 0)
		free(paths[i]);
	free(paths);
}

int	find_i(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
		if (ft_strnstr(envp[i], "PATH", 4) != 0)
			break ;
	return (i);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = find_i(envp);
	if (envp[i])
	{
		paths = ft_split(envp[i] + 5, ':');
		i = -1;
		while (paths[++i])
		{
			part_path = ft_strjoin(paths[i], "/");
			path = ft_strjoin(part_path, cmd);
			free(part_path);
			if (access(path, F_OK) == 0)
			{
				free_paths(paths);
				return (path);
			}
			free(path);
		}
		free_paths(paths);
	}
	return (0);
}

char	*change_directory(char *path)
{
	char	*slash;
	char	*final;
	char	*file;

	slash = ft_strchr(path, '/');
	while (slash)
	{
		final = slash;
		slash = ft_strchr(slash + 1, '/');
	}
	file = final + 1;
	*final = 0;
	chdir(path);
	*final = '/';
	return (file);
}
