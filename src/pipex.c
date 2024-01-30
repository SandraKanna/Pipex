/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/01/30 16:11:24 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*parse_cmd(char *path_var, char *command)
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
		full_path = ft_strjoin(partial_path, command);
		free(partial_path);
		if (access(full_path, F_OK) == 0)
		{
			// i = 0;
			// while (split_path[i])
			// 	free(split_path[i++]);
			// free(split_path);
			free_split(split_path);
			return (full_path);
		}
		i++;
	}
	free_split(split_path);
	return (NULL);
}

int	execute(char **av, char **envp, int index)
{
	char	*path_var;
	char	*full_cmd;
	char	**cmd_args;
	int		cmd_len;

	cmd_args = ft_split((const char *)av[index], ' ');
	cmd_len = ft_strlen(cmd_args[0]);
	if (cmd_len > 3 &&
		ft_strcmp(cmd_args[0] + ft_strlen(cmd_args[0]) - 3, ".sh") == 0)
	{
		full_cmd = ft_strjoin("sh ", cmd_args[0]); //rajputer "/"
		cmd_args[0] = full_cmd;
	}
	else
	{
		path_var = get_path_var(envp);
		full_cmd = parse_cmd(path_var, cmd_args[0]);
	}
	if (full_cmd == NULL)
		return (free_cmds(full_cmd, cmd_args, path_var), 0);
	else if (execve(full_cmd, cmd_args, envp) == -1)
		return (free_cmds(full_cmd, cmd_args, path_var), 0);
	return (1);
}

void	child_process(int *fd, char **av, char **envp, int index)
{
	int		in_file;
	int		bytes_read;
	char	buffer[1025];

	close(fd[0]);
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_INFILE);
	bytes_read = 1;
	if (ft_strcmp(av[1], "/dev/urandom") == 0)
	{
		while (bytes_read > 0)
		{
			bytes_read = read(in_file, buffer, 1024);
			write(fd[1], buffer, bytes_read);
		}
		if (bytes_read < 0)
			error_handling(EC_INFILE);
	}
	dup2(in_file, 0);
	close (in_file);
	dup2(fd[1], 1);
	close(fd[1]);
	if (!execute(av, envp, index))
		error_handling(EC_EXECVE);
}


/*void	child_process(int *fd, char **av, char **envp, int index)
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
	cmd1_path = parse_cmd(path_var, cmd1_args[0]);
	if (cmd1_path == NULL)
		error_handling(EC_EXECVE);
	dup2(in_file, 0);
	dup2(fd[1], 1);
	close(fd[1]);
	if (execve(cmd1_path, cmd1_args, envp) == -1)
	{
		free_cmds(cmd1_path, cmd1_args, path_var);
		error_handling(EC_EXECVE);
	}
}*/

void	parent_process(int *fd, int ac, char **av, char **envp)
{
	int		out_file;

	close(fd[1]);
	out_file = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(EC_OUTFILE);
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[0]);
	if (!execute(av, envp, ac - 2))
	{
		close (out_file);
		error_handling(EC_EXECVE);
	}
}

/*void	parent_process(int *fd, int ac, char **av, char **envp)
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
	cmd2_path = parse_cmd(path_var, cmd2_args[0]);
	if (cmd2_path == NULL)
		error_handling(EC_EXECVE);
	dup2(out_file, 1);
	dup2(fd[0], 0);
	close(fd[0]);
	if (execve(cmd2_path, cmd2_args, envp) == -1)
	{
		free_cmds(cmd2_path, cmd2_args, path_var);
		error_handling(EC_EXECVE);
	}

}*/

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
		// if (waitpid(p_id, NULL, 0) == -1)
		// 	error_handling(EC_WAIT);
		close(fd[1]);
		parent_process(fd, argc, argv, envp);
		close(fd[0]);
	}
	return (0);
}
