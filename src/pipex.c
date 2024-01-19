/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/01/18 18:25:48 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*parse_path(char *path_var, char *command)
{
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	int		i;

	if (check_abs_path(command))
		return (ft_strdup(command));
	split_path = ft_split((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, command);
		free(partial_path);
		if (access(full_path, F_OK) == 0)
		{
			i = 0;
			while (split_path[i])
				free(split_path[i++]);
			free(split_path);
			return (full_path);
		}
		i++;
	}
	return (NULL);
}

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;
	char	*path_var;

	close(fd[0]);
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_INFILE);
	cmd1_args = ft_split((const char *)av[2], ' ');
	path_var = get_path_var(envp);
	cmd1_path = parse_path(path_var, cmd1_args[0]);
	if (cmd1_path == NULL)
		error_handling(EC_EXECVE);
	dup2(in_file, 0);
	dup2(fd[1], 1);
	close(fd[1]);
	if (execve(cmd1_path, cmd1_args, envp) == -1)
	{
		free(cmd1_args);
		free(cmd1_path);
		free(path_var);
		error_handling(EC_EXECVE);
	}
}

void	parent_process(int *fd, char **av, char **envp)
{
	int		out_file;
	char	*cmd2_path;
	char	**cmd2_args;
	char	*path_var;

	close(fd[1]);
	out_file = open(av[4], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(EC_OUTFILE);
	cmd2_args = ft_split((const char *)av[3], ' ');
	path_var = get_path_var(envp);
	cmd2_path = parse_path(path_var, cmd2_args[0]);
	if (cmd2_path == NULL)
		error_handling(EC_EXECVE);
	dup2(out_file, 1);
	dup2(fd[0], 0);
	if (execve(cmd2_path, cmd2_args, envp) == -1)
	{
		free(cmd2_args);
		free(cmd2_path);
		free(path_var);
		close(out_file);
		close(fd[0]);
		error_handling(EC_EXECVE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	p_id;
	int		wstatus;

	if (argc != 5)
		error_handling(EC_ARGS);
	if (pipe(fd) < 0)
		error_handling(EC_PIPE);
	p_id = fork();
	if (p_id < 0)
		error_handling(EC_FORK);
	else if (p_id == 0)
		child_process(fd, argv, envp);
	else
	{
		if (waitpid(p_id, &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		close(fd[1]);
		parent_process(fd, argv, envp);
		close(fd[0]);
	}
	return (0);
}
