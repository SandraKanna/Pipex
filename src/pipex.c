/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/01/17 17:43:45 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;

	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
		error_handling(ERROR_INFILE);
	dup2(in_file, 0);
	dup2(fd[1], 1);
	close(fd[0]);
	//cmd1_args = parse_cmd(av[2]);
	cmd1_args = ft_split((const char *)av[2], ' ');
	cmd1_path = parse_path(cmd1_args[0], envp);
	if (!execve(cmd1_path, cmd1_args, envp))
	{	
		free(cmd1_args);
		free(cmd1_path);
		close(in_file);
		close(fd[1]);
		error_handling(ERROR_EXECVE);
	}
}

void	parent_process(int *fd, char **av, char **envp)
{
	int		out_file;
	char	*cmd2_path;
	char	**cmd2_args;

	out_file = open(av[4], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(ERROR_OUTFILE);
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[1]);
	//cmd2_args = parse_cmd(av[3]);
	cmd2_args = ft_split((const char *)av[3], ' ');
	cmd2_path = parse_path(cmd2_args[0], envp);
	if (!execve(cmd2_path, cmd2_args, envp))
	{
		free(cmd2_args);
		free(cmd2_path);
		close(out_file);
		close(fd[0]);
		error_handling(ERROR_EXECVE);
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	p_id;
	
	if (argc != 5)
		error_handling(ERROR_ARGS);
	if (pipe(fd) < 0)
		error_handling(ERROR_PIPE);
	p_id = fork();
	if (p_id < 0)
		error_handling(ERROR_FORK);
	else if (p_id == 0)
		child_process(fd, argv, envp);
	else
	{
		if (!waitpid(p_id, NULL, 0))
			error_handling(ERROR_WAIT);
		close(fd[1]);
		parent_process(fd, argv, envp);
		close(fd[0]);
	}
	return (0);
}
