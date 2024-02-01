/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/02/01 18:07:26 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
//#include "../libft_v2/original_src/libft.h"

char	*parse_cmd(char *path_var, char *command)
{
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	int		i;

	split_path = split_for_parse((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, command);
		free(partial_path);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0 && access(full_path, X_OK) == 0)
		{
			free_split(split_path);
			return (full_path);
		}
		free (full_path);
		i++;
	}
	if (split_path[i])
		free_split(split_path);
	return (NULL);
}

int	execute(char **av, char **envp, int index)
{
	char	*path_var;
	char	*cmd_path;
	char	**cmd_args;

	if (is_script(av[index]))
		av[index] = ft_strjoin("sh ", av[index]);
	path_var = get_path_var(envp);
	if (!path_var)
		return (0);
	cmd_args = split_for_parse((const char *)av[index], ' ');
	if (!cmd_args)
		return (free(path_var), 0);
	if (absolute_path(cmd_args[0]))
		cmd_path = cmd_args[0];
	else
	{
		cmd_path = parse_cmd(path_var, cmd_args[0]);
		if (!cmd_path)
			return (free_cmds(NULL, cmd_args, path_var), 0);
	}
	if (execve(cmd_path, cmd_args, envp) == -1)
		return (free_cmds(cmd_path, cmd_args, path_var), 0);
	return (1);
}

void	second_cmd(int *fd, int ac, char **av, char **envp)
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

void	first_cmd(int *fd, int ac, char **av, char **envp)
{
	int		in_file;

	close(fd[0]);
	in_file = open(av[ac - 4], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_INFILE);
	dup2(in_file, 0);
	close (in_file);
	dup2(fd[1], 1);
	close(fd[1]);
	if (!execute(av, envp, ac - 3))
		error_handling(EC_EXECVE);
}

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
	else if (p_id == 0) //child
		first_cmd(fd, argc, argv, envp);
	else //parent
	{
		close(fd[1]);
		second_cmd(fd, argc, argv, envp);
		if (waitpid(p_id, NULL, 0) == -1)
			error_handling(EC_WAIT);
		close(fd[0]);
	}
	return (0);
}
