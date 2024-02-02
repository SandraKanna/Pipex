/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/02/02 15:07:17 by skanna           ###   ########.fr       */
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
		if (!line || ft_strcmp(line, av[2]) == 0)
		{
			free(line);
			limiter = 1;
		}
		else if (line && ft_strcmp(line, av[2]) != 0)
		{
			write(hd_pipe[1], line, ft_strlen(line));
			free(line);
		}
		else
			error_handling(EC_INIT);
	}
	close(hd_pipe[1]);
	bonus->pipe_fd[0][0] = hd_pipe[0];
}

void	set_outfile(t_bonus *bonus, char **av, int ac)
{
	int		out_file;

	out_file = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(EC_OUTFILE);
	//dup2(out_file, 1);
	bonus->pipe_fd[bonus->cmd_count - 1][1] = out_file;
}

void	set_infile(t_bonus *bonus, char **av)
{
	int		in_file;

	in_file = 0;
	if (bonus->here_doc)
	{
		read_here_doc(bonus, av);
		//in_file = bonus->pipe_fd[0][0];
	}
	else
	{
		in_file = open (av[1], O_RDONLY);
		if (in_file < 0)
			error_handling(EC_INFILE);
		bonus->pipe_fd[0][0] = in_file;
	}
	//if (!bonus->here_doc)
	close (bonus->pipe_fd[0][1]);
}
