#include <unistd.h>
#include <fcntl.h>

int	main(int argc, char **argv, char **envp)
{
	
	//check for correct number of arguments
	if (argc != 5)
	{	
		ft_printf("Please enter the right parameters: ");
		exit (EXIT_FAILURE);
	}

	//create Pipe

	//open infile and create (fork) a new process (child = commandes) out of the calling process (parent = main)
	int in_file;
	pid_t   child1

	in_file = open(argv[1], O_RDONLY);
	child1 = fork();
	if (child1 < 0)
	{	
		perror("Error creating child1 process with fork")
		exit (EXIT_FAILURE)
	}
	if (child1 == 0)
	{
		//execve
		//int execve(const char *pathname, char *const argv[], char *const envp[]);
	}

	//open outfile and create (fork) a new process (child = commandes) out of the calling process (parent = main)
	int out_file;
	pid_t   child2
	
	out_file = open(argv[4], O_CREAT, O_WRONLY);
	child2 = fork();
	if (child2 < 0)
	{	
		perror("Error creating child2 process with fork")
		exit (EXIT_FAILURE)
	}

	//handle error

	if (child)
	{
		//redirect stdout to pipe
		//execute first command
	}
	else
	{
		//redirect stdin from pipe
		//execute second command
	}
	//close files and pipes
	// wait for child processes
	return (0);
}