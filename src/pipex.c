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

char	*merge_args(char **command_args)
{

}

char	**parse_cmd(char *command_str)
{
	char	**split_command;
	char	*final_command;
	char	quote;
	int		start;
	int		i;
	int		j;

	i = 0;
	start = 0;
	split_command = ft_split((char const *)command_str, ' ');
	while (split_command[i])
	{
		j = 0;
		while(split_command[i][j])
		{
			if (split_command[i][j] == 39 || split_command[i][j] == 34)
				start = 1;
			if (start == 1 && (split_command[i][j] == 39 || split_command[i][j] == 34))
				final_command = merge_args(split_command);
			j++;
		}
	}

	return (final_command);
}

char	*parse_path(char *command, char **envp)
{
	char	*path_var;
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	int		i;

	path_var = get_path_var(envp);
	if (path_var == NULL)
		error_handling(ERROR_PATH);
	split_path = ft_split((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
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
	cmd1_args = parse_cmd(av[2]);
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
	cmd2_args = parse_cmd(av[3]);
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
