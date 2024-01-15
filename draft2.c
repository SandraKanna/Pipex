#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int	ft_strnstr(const char *big, const char *little, int len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while ((j + i) < len && (little[i]) && (big[j]))
	{
		if (little[i] == big[j + i])
			i++;
		else
		{
			i = 0;
			j++;
		}
	}
	if (little[i] == '\0')
	{
		//printf("%s", big + j);
		return (1);
	}
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

char	*get_path(char **envp)
{
	int		i;
	int		check;
	char	*path;

	i = 0;
	check = 0;
	path = NULL;
	while (envp[i] && check != 1)
	{
		check = ft_strnstr((const char *)envp[i], "PATH=", 5);
		if (check)
			break;
		i++;
	}
	if (!envp[i])
	{	
		perror("ERROR can't get path");
		exit (EXIT_FAILURE);
	}
	else
		path = ft_strdup((const char *)envp[i]);
	//fprintf(stderr, "TOTO %s\n", path);
	return (path);
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
	path = get_path(envp);
	execve(path, &av[2], envp);
}

void	parent_process(int *fd, char **av, char **envp)
{
	int	out_file;
	char	*path;

	out_file = open(av[4], O_CREAT | O_RDWR);
	if (out_file < 0)
	{	
		perror("Error opening out_file");
		exit (EXIT_FAILURE);
	}
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[1]);
	path = get_path(envp);
	execve(path, &av[2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	int 	fd[2];
	pid_t	p_id;

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
		p_id = fork(); //create (fork) a new process (child = commandes) out of the calling process
		if (p_id < 0)
		{	
			perror("Error creating child1 process with fork");
			exit (EXIT_FAILURE);
		}
		if (p_id == 0)
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