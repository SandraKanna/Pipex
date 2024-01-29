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


void	bonus_child(t_pipex_bonus *bonus, char **av, char **env, int i)
{
	if (i == 0 && bonus->here_doc)
		dup2(bonus->input_hd, 0);
	else if (i != 0)
		dup2(bonus->pipe_fd[i - 1][0], 0);
	if (i != bonus->cmd_count - 1)
		dup2(bonus->pipe_fd[i][1], 1);
	else if (!bonus->here_doc)
		dup2();
	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		close (bonus->pipe_fd[i][0]);
		close (bonus->pipe_fd[i][1]);
		i++;
	}

}

void	fork_cmd_exec(t_pipex_bonus *bonus, char **av, char **env, int hd)
{
	int		i;

	i = 0;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
			bonus_child(&bonus, av, env, i);
		i++;
	}
}

void	create_pipes(t_pipex_bonus *bonus)
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

void	init_struct(t_pipex_bonus *bonus, int ac, char **av, int here)
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
	int				i;
	int				wstatus;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, av, bonus.here_doc);
	create_pipes(&bonus);
	set_infile(&bonus, av);
	set_outfile(&bonus, av, ac);
	fork_cmd_exec(&bonus, av, envp, bonus.here_doc);
	i = 0;
	while (i < bonus.cmd_count)
	{
		if (waitpid(bonus.pids[i], &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		i++;
	}
	return (0);
}

/*int	main(int ac, char **av, char **envp)
{
	t_pipex_bonus	bonus;
	//int				i;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, av, bonus.here_doc);
	create_pipes(&bonus);
	if (bonus.here_doc)
		here_doc_exec();
	else
		classic_exec(&bonus, av, envp);
	
	return (0);
}*/