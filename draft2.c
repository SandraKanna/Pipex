#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_strnstr(const char *big, const char *little, int len)
{
	int	i;

	i = 0;
    if (little[0] == '\0') //if there's no path, return 0
        return 0;
	while ((little[i]) && (big[i]) && i < len) //check for "PATH=" only at the beginning of big
	{
		if (little[i] != big[i])
			return (0);
		i++;
	}
	if (little[i] == '\0')
		return (1);
	else
		return (0);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		len_s;
	int		i;

	i = 0;
	len_s = ft_strlen(s);
	dup = malloc((len_s + 1) * sizeof(const char));
	if (dup == NULL)
		return (NULL);
	while (s[i])
	{
		dup[i] = ((const char *)s)[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*get_path_var(char **envp)
{
	int		i;
	int		check;
	char	*path;

	i = 0;
	check = 0;
	path = NULL;
	while (envp[i] && check != 1)
	{
		if (ft_strnstr((const char *)envp[i], "PATH=", 5))
		{
			path = ft_strdup((const char *)envp[i] + 5);
			break;
		}
		i++;
	}
	if (path == NULL)
	{	
		perror("ERROR: PATH environment variable not found\n");
		exit (EXIT_FAILURE);
	}
	//fprintf(stderr, "TOTO %s\n", path);
	return (path);
}

char	*parse_path(char *command, char **envp)
{
	char	*path_var;
	char	*command_path;

	path_var = get_path_var(envp);
    // parse 'path_var' and find the full path of the 'command'
    // split 'path_var' into individual directories
    // check if 'command' exists in these directories

	return (command_path);
	
}

char	**parse_command(char *cmd)
{
    char	**parsed;
	
	// Split 'cmd' into an array of strings
    // parsed[0] = command name, parsed[1] = first argument/option, etc

	return(parsed);
}

void	child_process(int *fd, char **av, char **envp)
{
	int		in_file;
	char	*cmd1_path;
	char	**cmd1_args;

	printf("Child process started\n");
	cmd1_args = parse_command(av[2]); //need to break down *av[2] into the command and its potential options, ex: ls -l
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
	{	
		perror("Error opening in_file\n");
		exit (EXIT_FAILURE);
	}
	dup2(in_file, 0); //redirects the content from in_file to the stdin
	dup2(fd[1], 1); //takes the content read from the stdin (written to fd[1]) and redirects it to the pipe
	close(fd[0]); //close it because child doesnt need to read it anymore after dup2
	close (in_file);
	cmd1_path = parse_path(cmd1_args[0], envp); //need to break down the path so we have only the command (and its options) path
	execve(cmd1_path, cmd1_args, envp);
}

void	parent_process(int *fd, char **av, char **envp)
{
	int	out_file;
	char	*cmd2_path;
	char	**cmd2_args;

	printf("Parent process started\n");
	cmd2_args = parse_command(av[3]); //need to break down *av[3] into the command and its potential options, ex: grep -i "example" file_name
	out_file = open(av[4], O_CREAT | O_RDWR, 0666);
	if (out_file < 0)
	{	
		perror("Error opening out_file\n");
		exit (EXIT_FAILURE);
	}
	dup2(out_file, 1); //redirects the content from out_file to the stdout
	dup2(fd[0], 0); 
	close(fd[1]); //closing the write end because the parent only needs to read from the pipe
	cmd2_path = parse_path(cmd2_args[0], envp); //need to break down the path so we have only the command (and its options) path
	execve(cmd2_path, cmd2_args, envp);
}

int	main(int argc, char **argv, char **envp)
{
	int 	fd[2];
	pid_t	p_id;

	//check for correct number of arguments
	if (argc == 5)
	{
		//create pipe and check for errors
		if (pipe(fd) < 0)
		{	
			perror("Error when creating pipe\n");
			exit (EXIT_FAILURE);
		}
		p_id = fork(); //create (fork) a new process (child = commandes) out of the calling process
		if (p_id < 0)
		{	
			perror("Error creating child1 process with fork\n");
			exit (EXIT_FAILURE);
		}
		if (p_id == 0)
			child_process(fd, argv, envp);
		waitpid(p_id, NULL, 0); //NULL == not interested in the exit status of the child process, 0 == default process == wait for child process to be finished
		close(fd[1]); //closing the write end of the pipe because it was already used in child process
		parent_process(fd, argv, envp);
		close(fd[0]);
	}
	else
	{	
		perror("Please enter the right parameters: file1 cmd1 cmd2 file2\n");
		exit (EXIT_FAILURE);
	}
	return (0);
}
