/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/01/16 16:23:52 by skanna           ###   ########.fr       */
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
	split_path = ft_split((char const *)path_var, ':'); // split 'path_var' into individual directories
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, command);
		if (access(full_path, F_OK) == 0)
			return(full_path);
		i++;
	}
	return (NULL);
}

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;

	printf("Child process started\n");
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
	{	
		perror("Error opening in_file");
		exit (EXIT_FAILURE);
	}
	dup2(in_file, 0); //reads and redirects the content from in_file to the stdin
	dup2(fd[1], 1); //takes the content read from the stdin (written to fd[1]) and redirects it to the pipe
	close(fd[0]); //close it because child doesnt need to read it anymore after dup2
	close(in_file);
	close(fd[1]);
	cmd1_args = ft_split((char const *)av[2], ' ');
	cmd1_path = parse_path(cmd1_args[0], envp); //need to break down the path so we have only the command (and its options) path
	execve(cmd1_path, cmd1_args, envp);
	free(cmd1_args);
	free(cmd1_path);
}

void	parent_process(int *fd, char **av, char **envp)
{
	int		out_file;
	char	*cmd2_path;
	char	**cmd2_args;

	printf("Parent process started\n");
	out_file = open(av[4], O_CREAT | O_RDWR, 0666);
	if (out_file < 0)
	{	
		perror("Error opening out_file");
		exit (EXIT_FAILURE);
	}
	dup2(out_file, 1); //redirects the content from out_file to the stdout
	dup2(fd[0], 0); 
	close(fd[1]); //closing the write end because the parent only needs to read from the pipe
	close(out_file);
	close(fd[0]);
	cmd2_args = ft_split((char const *)av[3], ' ');
	cmd2_path = parse_path(cmd2_args[0], envp);
	execve(cmd2_path, cmd2_args, envp);
	free(cmd2_args);
	free(cmd2_path);
}

int	main(int argc, char **argv, char **envp)
{
	int 	fd[2];
	pid_t	p_id;

	if (argc == 5)
	{
		//create pipe and check for errors
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
			waitpid(p_id, NULL, 0); //NULL == not interested in the exit status of the child process, 0 == default process == wait for child process to be finished
			close(fd[1]); //closing the write end of the pipe because it was already used in child process
			parent_process(fd, argv, envp);
			close(fd[0]);
		}
	}
	else
	{	
		perror("Please enter the right parameters: file1 cmd1 cmd2 file2");
		exit (EXIT_FAILURE);
	}
	return (0);
}
