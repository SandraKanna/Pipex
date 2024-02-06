/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/02/06 19:05:28 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/pipex.h"

char	*get_path_var(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	path = NULL;
	while (envp[i])
	{
		if (ft_strnstr((const char *)envp[i], "PATH=", 5))
		{
			path = ft_strdup((const char *)envp[i] + 5);
			if (!path)
				return (NULL);
		}
		i++;
	}
	return (path);
}

char	*build_path(char *path, char *command)
{
	char	*partial_path;

	partial_path = ft_strjoin(path, "/");
	if (!partial_path)
		return (NULL);
	path = ft_strjoin(partial_path, command);
	free(partial_path);
	if (!path)
		return (NULL);
	if (access(path, F_OK) == 0 && access(path, X_OK) == 0)
		return (path);
	return (free(path), NULL);
}

char	*parse_cmd(char *path_var, char *command)
{
	char	**split_path;
	char	*full_path;
	int		i;

	split_path = split_for_parse((char const *)path_var, ':');
	if (!split_path)
		return (NULL);
	i = 0;
	while (split_path[i])
	{
		full_path = build_path(split_path[i], command);
		if (full_path)
		{
			free_split(split_path);
			return (full_path);
		}
		free (full_path);
		i++;
	}
	free_split(split_path);
	return (NULL);
}

int	execute(char **av, char **envp, int index)
{
	char	*path_var;
	char	*cmd_path;
	char	**cmd_args;

	if (is_script(av[index]))
		av[index] = ft_strjoin("sh ", av[index]);
	path_var = get_path_var(envp);
	if (!path_var)
		return (0);
	cmd_args = split_for_parse((const char *)av[index], ' ');
	if (!cmd_args)
		return (free(path_var), 0);
	if (absolute_path(cmd_args[0]))
		cmd_path = cmd_args[0];
	else
	{
		cmd_path = parse_cmd(path_var, cmd_args[0]);
		if (!cmd_path)
			return (free_cmds(NULL, cmd_args, path_var), 0);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
		return (free_cmds(cmd_path, cmd_args, path_var), 0);
	return (1);
}
