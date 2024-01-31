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
#include "../libft_v2/original_src/libft.h"

/*int	execute_helper(char *path_var, char **env, char **cmd_args, int cmd_len)
{
	char	*full_cmd;
	char	*cmd_sh;

	cmd_sh = NULL;
	if (cmd_len > 3 && ft_strcmp(cmd_args[0]
			+ ft_strlen(cmd_args[0]) - 3, ".sh") == 0)
	{
		cmd_sh = ft_strjoin("sh ", cmd_args[0]);
		if (!cmd_sh)
			return (0);
		full_cmd = parse_cmd(path_var, cmd_sh);
		free (cmd_args[0]);
		cmd_args[0] = cmd_sh;
	}
	else
		full_cmd = parse_cmd(path_var, cmd_args[0]);
	if (!full_cmd)
	{
		free (cmd_sh);
		return (free_cmds(NULL, cmd_args, path_var), 0);
	}
	return (execve(full_cmd, cmd_args, env) != -1);
}

int	execute(char **av, char **envp, int index)
{
	char	*path_var;
	char	**cmd_args;
	int		cmd_len;
	int		has_executed;

	path_var = get_path_var(envp);
	cmd_args = split_for_parse((const char *)av[index], ' ');
	if (!cmd_args)
	{
		free (path_var);
		return (0);
	}
	cmd_len = ft_strlen(cmd_args[0]);
	has_executed = execute_helper(path_var, envp, cmd_args, cmd_len);
	free_cmds(NULL, cmd_args, path_var);
	return (has_executed);
}*/

int	execute(char **av, char **envp, int index)
{
	char	*path_var;
	char	*full_cmd;
	char	**cmd_args;

	path_var = get_path_var(envp);
	cmd_args = split_for_parse((const char *)av[index], ' ');
	if (!cmd_args)
		return (free(path_var), 0);
	if (check_abs_path(cmd_args[0]))
		full_cmd = cmd_args[0];
	else
	{
		full_cmd = parse_cmd(path_var, cmd_args[0]);
		if (!full_cmd)
		{	
			//return (free_cmds(NULL, cmd_args, path_var), 0);
			//free_split(cmd_args);
			free(path_var);
			return 0;
		}
	}
	if (execve(full_cmd, cmd_args, envp) == -1)
		return (free_cmds(full_cmd, cmd_args, path_var), 0);
		return (0);
	return (1);
}

void	child_process(int *fd, char **av, char **envp, int index)
{
	int		in_file;

	close(fd[0]);
	in_file = open(av[1], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_INFILE);
	dup2(in_file, 0);
	close (in_file);
	dup2(fd[1], 1);
	close(fd[1]);
	if (!execute(av, envp, index))
		error_handling(EC_EXECVE);
}

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
		close(fd[1]);
		parent_process(fd, argc, argv, envp);
		close(fd[0]);
	}
	return (0);
}
