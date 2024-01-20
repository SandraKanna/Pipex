/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/01/19 17:28:59 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"



int	classic_exec(t_pipex_bonus *b_struc, char **av, char **env)
{
	int	i;

	i = 0;
	while (i < b_struc->cmd_count - 1)
	{
		if (pipe(b_struc->pipe_fd[i]) < 0)
			error_handling(EC_PIPE);
		i++;
	}
	i = 0;
	while (i < b_struc->cmd_count)
	{
		b_struc->pids[i] = fork();
		if (b_struc->pids[i] < 0)
			error_handling(EC_FORK);
		else if (b_struc->pids[i] == 0)
			child_process(b_struc->pids[i], av, env, i);
	}

	if (waitpid(b_struc->pids[i], NULL, 0) == -1)
		error_handling(EC_WAIT);
}

void	init_struct(t_pipex_bonus *b_struc, int ac, char **av, int here)
{
	int	i;

	i = 0;
	if (here)
	{
		b_struc->cmd_count = ac - 4;
		init_here_doc(b_struc, av);
	}
	else
		b_struc->cmd_count = ac - 3;
	b_struc->pipe_fd = malloc (sizeof(int *) * (b_struc->cmd_count - 1));
	b_struc->pids = malloc (sizeof(pid_t) * (b_struc->cmd_count));
	if (!b_struc->pipe_fd || !b_struc->pids)
		error_handling(EC_PIPE);
	while (i < b_struc->cmd_count - 1)
	{
		b_struc->pipe_fd[i] = malloc (sizeof(int) * 2);
		if (!b_struc->pipe_fd[i])
			error_handling(EC_PIPE);
		i++;
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex_bonus	pipex_bonus;

	if (ac < 5)
		error_handling(EC_ARGS);
	pipex_bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&pipex_bonus, ac, av, pipex_bonus.here_doc);
	if (pipex_bonus.here_doc)
		here_doc_exec();
	else
		classic_exec(&pipex_bonus, ac, av);
	clean_stuct();
	return (0);
}
