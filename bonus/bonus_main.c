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

#include "../Includes/bonus.h"

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