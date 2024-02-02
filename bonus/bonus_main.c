/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/02 18:03:37 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

static void	init_struct(t_bonus *bonus, int ac, int here)
{
	int	i;

	i = 0;
	if (here)
		bonus->cmd_count = ac - 4;
	else
		bonus->cmd_count = ac - 3;
	bonus->pipe_fd = malloc (sizeof(int *) * (bonus->cmd_count));
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
	int		wstatus;
	int		i;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, bonus.here_doc);
	create_pipes(&bonus);
	setup_n_exec(&bonus, av, envp, ac);
	i = 0;
	while (i < bonus.cmd_count)
	{
		if (waitpid(bonus.pids[i], &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		i++;
	}
	free (bonus.pids);
	return (0);
}
