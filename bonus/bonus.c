/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/01 18:15:07 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

//https://github.com/mcombeau/pipex

void	exec_bonus(t_bonus *bonus, char **av, char **envp, int index)
{
	int	i;

	i = 0;
	while (bonus->pipe_fd[i] && (i < bonus->cmd_count - 1))
	{
		//children
		dup2(bonus->pipe_fd[i][0], 0);
		close(bonus->pipe_fd[i][0]);
		dup2(bonus->pipe_fd[i][1], 1);
		close(bonus->pipe_fd[i][1]);
		if (!execute(av, envp, index))
			error_handling(EC_EXECVE);
		i++;
	}
	//last command
	if (!bonus->pipe_fd[i])
		error_handling(EC_EXECVE);
	dup2(bonus->pipe_fd[i][1], 1);
	close(bonus->pipe_fd[i][1]);
	dup2(bonus->pipe_fd[i][0], 0);
	close(bonus->pipe_fd[i][0]);
	if (!execute(av, envp, index))
		error_handling(EC_EXECVE);
}

void	fork_children(t_bonus *bonus, char **av, char **env)
{
	int		i;
	int		index;

	i = 0;
	index = 2;
	if (bonus->here_doc)
		index += 1;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
			exec_bonus(bonus, av, env, index);
		else if (bonus->pids[i] > 0)
			break ;
		i++;
	}
}

void	create_pipes(t_bonus *bonus)
{
	int	i;

	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		if (pipe(bonus->pipe_fd[i]) < 0)
			error_handling(EC_PIPE);
		i++;
	}
}

void	init_struct(t_bonus *bonus, int ac, int here)
{
	int	i;

	i = 0;
	if (here)
		bonus->cmd_count = ac - 4;
	else
		bonus->cmd_count = ac - 3;
	bonus->pipe_fd = malloc (sizeof(int *) * (bonus->cmd_count - 1));
	bonus->pids = malloc (sizeof(pid_t) * (bonus->cmd_count));
	if (!bonus->pipe_fd || !bonus->pids)
		error_handling(EC_MEM);
	while (i < bonus->cmd_count)
	{
		bonus->pipe_fd[i] = malloc (sizeof(int) * 2);
		if (!bonus->pipe_fd[i])
			error_handling(EC_MEM);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_bonus	bonus;
	int		i;
	int		wstatus;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, bonus.here_doc);
	create_pipes(&bonus);
	set_infile(&bonus, av);
	set_outfile(&bonus, av, ac);
	fork_children(&bonus, av, envp);
	//execute_parent();
	i = 0;
	while (i < bonus.cmd_count)
	{
		if (waitpid(bonus.pids[i], &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		i++;
	}
	return (0);
}
