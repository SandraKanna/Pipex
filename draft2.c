#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

char	*get_path()
{
	
}

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*path;

	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
	{	
		perror("Error opening in_file");
		exit (EXIT_FAILURE);
	}
	dup2(in_file, 0);
	dup2(fd[1], 1);
	close(fd[0]);
	path = get_path();
	execve(path, &av[2], envp);
}

void	parent_process(int *fd, char **av, char **envp)
{
	int	out_file;

	out_file = open(av[4], O_CREAT | O_RDWR);
	if (out_file < 0)
	{	
		perror("Error opening out_file");
		exit (EXIT_FAILURE);
	}
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[1]);
	execve(path, &av[2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int 	fd[2];
	pid_t	p_id;

	p_id = fork(); //create (fork) a new process (child = commandes) out of the calling process
	pipe(fd); //create Pipe
	//check for correct number of arguments
	if (argc == 5)
	{
		//check pipe errors
		if (pipe(fd) < 0)
		{	
			perror("Error when creating pipe");
			exit (EXIT_FAILURE);
		}
		if (p_id < 0)
		{	
			perror("Error creating child1 process with fork");
			exit (EXIT_FAILURE);
		}
		if (!p_id) // same than p_id == 0
			child_process(fd, argv, envp);
		//waitpid
		parent_process(fd, argv, envp);
	}
	else
	{	
		perror("Please enter the right parameters: file1 cmd1 cmd2 file2");
		exit (EXIT_FAILURE);
	}
	return (0);
}