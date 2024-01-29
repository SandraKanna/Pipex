/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 12:05:14 by skanna           ###   ########.fr       */
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
		return (command);
	split_path = ft_split((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		//para err 8: agregar sh al comando ej: sh ./script.sh, buscabdo si len arg >3 y los ultimos chars son .sh => join sh + cmd
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

// for err TO: GNL function to read to *fd (sent by child) until theres no more to read (buff = NULL)
// or we've reached Buffer_Size (strlen(buff)> buffer size)
void	child_process(int *fd, char **av, char **envp, int index)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;
	char	*path_var;

	close(fd[0]);
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_INFILE);
	cmd1_args = ft_split((const char *)av[index], ' ');
	path_var = get_path_var(envp);
	cmd1_path = parse_path(path_var, cmd1_args[0]);
	if (cmd1_path == NULL)
		error_handling(EC_EXECVE);
	dup2(in_file, 0);
	//GNL to *fd
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

void	parent_process(int *fd, int ac, char **av, char **envp)
{
	int		out_file;
	char	*cmd2_path;
	char	**cmd2_args;
	char	*path_var;

	close(fd[1]);
	out_file = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(EC_OUTFILE);
	cmd2_args = ft_split((const char *)av[ac - 2], ' ');
	path_var = get_path_var(envp);
	cmd2_path = parse_path(path_var, cmd2_args[0]);
	if (cmd2_path == NULL)
		error_handling(EC_EXECVE);
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[0]);
	if (execve(cmd2_path, cmd2_args, envp) == -1)
	{
		free(cmd2_args);
		free(cmd2_path);
		free(path_var);
		close(out_file);
		error_handling(EC_EXECVE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	p_id;

	if (argc != 5)
		error_handling(EC_ARGS);
	if (pipe(fd) < 0)
		error_handling(EC_PIPE);
	p_id = fork();
	if (p_id < 0)
		error_handling(EC_FORK);
	else if (p_id == 0)
		child_process(fd, argv, envp, 2);
	else
	{
		if (waitpid(p_id, NULL, 0) == -1)
			error_handling(EC_WAIT);
		close(fd[1]);
		parent_process(fd, argc, argv, envp);
		close(fd[0]);
	}
	return (0);
}
