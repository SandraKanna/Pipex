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

char	*parse_path(char *command, char **envp)
{
	char	*path_var;
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	int		i;

	path_var = get_path_var(envp);
	split_path = ft_split((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, command);
		free(partial_path);
		if (access(full_path, F_OK) == 0)
			return (full_path);
		i++;
	}
	return (NULL);
}

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;

	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
	{
		perror("Error opening in_file");
		exit (EXIT_FAILURE);
	}
	dup2(in_file, 0);
	dup2(fd[1], 1);
	close(fd[0]);
	close(in_file);
	close(fd[1]);
	cmd1_args = ft_split((char const *)av[2], ' ');
	cmd1_path = parse_path(cmd1_args[0], envp);
	execve(cmd1_path, cmd1_args, envp);
	//	perror("Error executing execve");
	free(cmd1_args);
	free(cmd1_path);
}

void	parent_process(int *fd, char **av, char **envp)
{
	int		out_file;
	char	*cmd2_path;
	char	**cmd2_args;

	//printf("Parent process started\n");
	out_file = open(av[4], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
	{	
		perror("Error opening out_file");
		exit (EXIT_FAILURE);
	}
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[1]);
	close(out_file);
	close(fd[0]);
	cmd2_args = ft_split((char const *)av[3], ' ');
	cmd2_path = parse_path(cmd2_args[0], envp);
	execve(cmd2_path, cmd2_args, envp);
	//	perror("Error executing execve");
	free(cmd2_args);
	free(cmd2_path);
}

void	pipex(char **argv, char **envp)
{
	int		fd[2];
	pid_t	p_id;

	if (pipe(fd) < 0)
	{
		perror("Error when creating pipe");
		exit (EXIT_FAILURE);
	}
	p_id = fork();
	if (p_id < 0)
	{
		perror("Error creating child1 process with fork");
		exit (EXIT_FAILURE);
	}
	else if (p_id == 0)
		child_process(fd, argv, envp);
	else
	{
		waitpid(p_id, NULL, 0);
		close(fd[1]);
		parent_process(fd, argv, envp);
		close(fd[0]);
	}
}

int	main(int argc, char **argv, char **envp)
{
	if (argc != 5)
	{
		perror("Please enter the right parameters: file1 cmd1 cmd2 file2");
		exit (EXIT_FAILURE);
	}
	pipex(argv, envp);
	return (0);
}
