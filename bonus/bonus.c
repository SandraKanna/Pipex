/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/05 19:04:04 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

static void	exec_cmd(t_bonus *bonus, char **av, char **envp, int i)
{
	int	index;

	index = 2 + i;
	if (bonus->here_doc)
		index += 1;
	close_fds(bonus, i);
	if (!execute(av, envp, index))
		error_handling(EC_EXECVE);
}

static void	dup_fds(t_bonus *bonus, int i)
{
	if (i == 0)
	{
		if (dup2(bonus->infile_fd, 0) < 0)
			error_handling(EC_IN);
		close(bonus->infile_fd);
	}
	else
	{
		if (dup2(bonus->pipe_fd[i - 1][0], 0) < 0)
			error_handling(EC_IN);
		close(bonus->pipe_fd[i - 1][0]);
	}
	if (i == bonus->cmd_count - 1)
	{
		if (dup2(bonus->outfile_fd, 1) < 0)
			error_handling(EC_IN);
		close(bonus->outfile_fd);
	}
	else
	{
		if (dup2(bonus->pipe_fd[i][1], 1) < 0)
			error_handling(EC_IN);
		close(bonus->pipe_fd[i][1]);
	}
}

void	setup_n_exec(t_bonus *bonus, char **av, char **env, int ac)
{
	int		i;

	i = 0;
	set_infile(bonus, av);
	set_outfile(bonus, av, ac);
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
		{
			dup_fds(bonus, i);
			close_fds(bonus, i);
			exec_cmd(bonus, av, env, i);
			exit (EXIT_SUCCESS);
		}
		i++;
	}
	close_fds(bonus, i);
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
