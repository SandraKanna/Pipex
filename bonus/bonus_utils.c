/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/05 18:59:29 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

void	read_here_doc(t_bonus *bonus, char **av)
{
	int		hd_pipe[2];
	char	*line;
	int		limiter;

	if (pipe(hd_pipe) < 0)
		error_handling(EC_PIPE);
	limiter = 0;
	while (limiter == 0)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strncmp(line, av[2], ft_strlen(av[2])) == 0)
		{
			free(line);
			limiter = 1;
		}
		else if (line && ft_strncmp(line, av[2], ft_strlen(av[2])) != 0)
		{
			write(hd_pipe[1], line, ft_strlen(line));
			free(line);
		}
		else
			error_handling(EC_INIT);
	}
	close(hd_pipe[1]);
	bonus->infile_fd = hd_pipe[0];
}

void	set_outfile(t_bonus *bonus, char **av, int ac)
{
	bonus->outfile_fd = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (bonus->outfile_fd < 0)
		error_handling(EC_OUT);
}

void	set_infile(t_bonus *bonus, char **av)
{
	if (bonus->here_doc)
		read_here_doc(bonus, av);
	else
	{
		bonus->infile_fd = open(av[1], O_RDONLY);
		if (bonus->infile_fd < 0)
			error_handling(EC_IN);
	}
}

void	close_fds(t_bonus *bonus, int current_cmd)
{
	int	i;

	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		if (i != current_cmd - 1)
			close(bonus->pipe_fd[i][0]);
		if (i != current_cmd)
			close(bonus->pipe_fd[i][1]);
		i++;
	}
}
