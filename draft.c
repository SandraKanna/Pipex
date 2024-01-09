/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 14:07:53 by skanna            #+#    #+#             */
/*   Updated: 2023/12/15 14:08:00 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*int	main(int argc, char **argv)
{
	if (argc == 5)
	{
		//read the content of file1 = argv[1]
		read (argv[1], buffer, BUFFER_SIZE);
		//argv[2] = cmd1;
		// argv[3] = cmd2;
		// argv[4] = file2;
		
	}
}*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int infile_fd;
	int outfile_fd;
	int pipe_fd[2];
	pid_t child_pid;

	if (argc != 5)
	{
		printf("Parameters should be entered like this: %s infile command1 command2 outfile\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// Open file1 for reading

	infile_fd = open(argv[1], O_RDONLY);
	if (infile_fd == -1) 
	{
		perror("open infile");
		exit(EXIT_FAILURE);
	}
	// Create a pipe for communication between command1 and command2
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	// Fork the process
	child_pid = fork();
	if (child_pid == -1) 
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (child_pid == 0) 
	{
		// Child process (command1)
		// Close write end of the pipe
		close(pipe_fd[0]);

		// Redirect stdin to read from file1
		dup2(infile_fd, STDIN_FILENO);

		// Redirect stdout to write to the pipe
		dup2(pipe_fd[1], STDOUT_FILENO);

		// Close unused file descriptors
		close(infile_fd);
		close(pipe_fd[1]);

		// Execute command1
		if (execve(argv[2], &argv[2], NULL) == -1)
		{
			perror("can't execute command1");
			exit(EXIT_FAILURE);
		}
	} 
	else 
	{
		// Parent process
		// Close read end of the pipe
		close(pipe_fd[1]);

		// Wait for the child process (command1) to finish
		waitpid(child_pid, NULL, 0);

		// Open file2 for writing
		outfile_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if (outfile_fd == -1)
		{
			perror("open outfile");
			exit(EXIT_FAILURE);
		}

		// Redirect stdin to read from the pipe
		dup2(pipe_fd[0], STDIN_FILENO);

		// Redirect stdout to write to file2
		dup2(outfile_fd, STDOUT_FILENO);

		// Close unused file descriptors
		close(pipe_fd[0]);
		close(outfile_fd);

		// Execute command2
		if (execve(argv[3], &argv[3], NULL) == -1)
		{
			perror("can't execut command2");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}
