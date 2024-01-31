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

int	is_script(char *cmd)
{
	int		cmd_len;

	cmd_len = ft_strlen(cmd);
	if (cmd_len > 3 &&
		ft_strcmp(cmd + cmd_len - 3, ".sh") == 0)
		return (1);
	return (0);
}

int	absolute_path(char *cmd)
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
