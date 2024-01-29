/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 15:04:11 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

//https://github.com/mcombeau/pipex

// for err TO: GNL function to read to *fd (sent by child) until theres no more to read (buff = NULL)
// or we've reached Buffer_Size (strlen(buff)> buffer size)

char parse_command()
{
	char	*cmd1_path;
	char	**cmd1_args;
	char	*path_var;
	
	cmd1_args = ft_split((const char *)av[index], ' ');
	path_var = get_path_var(envp);
	cmd1_path = parse_path(path_var, cmd1_args[0]);
	if (cmd1_path == NULL)
		error_handling(EC_EXECVE);
}

void	exec()
{
	
}

void	classic_exec(t_pipex_bonus *bonus, char **av, char **env)
{
	int		i;
	
	i = 0;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
		{

		//	exec(bonus->pids[i], av, env, i);
		}
	}
	if (waitpid(bonus->pids[i], NULL, 0) == -1)
		//error_handling(EC_WAIT);
}

void	create_pipes(t_pipex_bonus *bonus)
{
	int	i;

	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		if (pipe(bonus->pipe_fd[i]) < 0)
		{
			clean_stuct(?);
			error_handling(EC_PIPE);
		}
		i++;
	}
}

void	init_struct(t_pipex_bonus *bonus, int ac, char **av, int here)
{
	int	i;

	i = 0;
	if (here)
	{
		bonus->cmd_count = ac - 4;
		init_here_doc(bonus, av);
	}
	else
		bonus->cmd_count = ac - 3;
	bonus->pipe_fd = malloc (sizeof(int *) * (bonus->cmd_count - 1));
	bonus->pids = malloc (sizeof(pid_t) * (bonus->cmd_count));
	if (!bonus->pipe_fd || !bonus->pids)
		error_handling(EC_PIPE);
	while (i < bonus->cmd_count - 1)
	{
		bonus->pipe_fd[i] = malloc (sizeof(int) * 2);
		if (!bonus->pipe_fd[i])
			error_handling(EC_PIPE);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex_bonus	bonus;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, av, bonus.here_doc);
	create_pipes(&bonus);
	if (bonus.here_doc)
		here_doc_exec();
	else
		classic_exec(&bonus, av, envp);
	clean_stuct(?);
	return (0);
}
