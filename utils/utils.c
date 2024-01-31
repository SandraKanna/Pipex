/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:48 by skanna            #+#    #+#             */
/*   Updated: 2024/01/30 15:46:11 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pipex.h"
#include "../libft_v2/original_src/libft.h"

char	*parse_sh(char *command)
{
	int		cmd_len;

	cmd_len = ft_strlen(command);
	if (cmd_len > 3)
	{
		if (command[cmd_len - 1] == 'h' && command[cmd_len - 2] == 's'
			&& command[cmd_len - 3] == '.')
			return (ft_strjoin("sh ", command));
	}
	//&& ft_strcmp(command + ft_strlen(command) - 3, ".sh") == 0)
	return (command);
}

char	*parse_cmd(char *path_var, char *command)
{
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	char	*filter_script;
	int		i;

	filter_script = parse_sh(command);
	if (!filter_script)
		return (NULL);
	split_path = split_for_parse((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, filter_script);
		if (!full_path)
			return (NULL);
		free(partial_path);
		if (access(full_path, F_OK) == 0)
		{
			free (filter_script);
			free_split(split_path);
			return (full_path);
		}
		free (full_path);
		i++;
	}
	free (filter_script);
	free_split(split_path);
	return (NULL);
}

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

int	check_abs_path(char *cmd)
{
	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
			return (1);
	}
	return (0);
}

int	error_handling(t_error_code error_code)
{
	const char	*error_msg[] = {ERROR_ARGS, ERROR_PIPE, ERROR_FORK,
		ERROR_WAIT, ERROR_INFILE, ERROR_OUTFILE, ERROR_EXECVE,
		ERROR_MEM};

	perror(error_code[error_msg]);
	if (error_code == EC_EXECVE)
		exit(127);
	else
		exit(EXIT_FAILURE);
}

void	free_cmds(char *full_cmd, char **cmd_args, char *path)
{
	if (full_cmd != NULL)
		free(full_cmd);
	if (cmd_args != NULL)
		free_split(cmd_args);
	if (path != NULL)
		free(path);
}
