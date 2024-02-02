/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/02 15:07:18 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

//https://github.com/mcombeau/pipex


void	exec_bonus(t_bonus *bonus, char **av, char **envp, int i)
{
	int	index;
	int	j;
	
	index = 2
	if (bonus->here_doc)
		index += 1;
	j = 0;
	while (i < bonus->cmd_count)
	{
		//close fds not used
	}
	if (!execute(av, envp, index))
		error_handling(EC_EXECVE);
}

void	dup_fds(t_bonus *bonus, char **av, char **envp)
{
	int	i;

	i = 0;
	while (bonus->pipe_fd[i] && (i < bonus->cmd_count))
	{
		if (i == 0)
		{
			dup2(bonus->pipe_fd[i][0], 0);
			close(bonus->pipe_fd[i][0]);
		}
		else
		{
			dup2(bonus->pipe_fd[i - 1][0], 0);
			close(bonus->pipe_fd[i - 1][0]);
		}
		if (i == bonus->cmd_count - 1)
		{
			dup2(bonus->pipe_fd[i][1], 1);
			close(bonus->pipe_fd[i][1]);
		}
		exec_bonus(bonus, av, envp, i);
		i++;
	}
}

void	fork_children(t_bonus *bonus, char **av, char **env)
{
	int		i;
	int		index;

	i = 0;
	set_infile(&bonus, av);
	set_outfile(&bonus, av, ac);
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
			dup_fds(bonus, av, env);
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
