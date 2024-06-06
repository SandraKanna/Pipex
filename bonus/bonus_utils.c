/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sandra <sandra@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/06/07 00:00:17 by sandra           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/bonus.h"

static int	process_input_line(int *hd_pipe, char *line, const char *del)
{
	size_t	line_len;
	size_t	del_len;

	line_len = ft_strlen(line);
	del_len = ft_strlen(del);
	if (line_len > 0 && line[line_len - 1] == '\n')
	{
		line[line_len - 1] = '\0';
		line_len--;
	}
	if (ft_strncmp(line, del, del_len) == 0 && line_len == del_len)
	{
		free(line);
		return (1);
	}
	line[line_len] = '\n';
	line_len++;
	write(hd_pipe[1], line, line_len);
	free(line);
	return (0);
}

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
		if (!line)
			limiter = 1;
		else
			limiter = process_input_line(hd_pipe, line, av[2]);
	}
	close(hd_pipe[1]);
	bonus->infile_fd = hd_pipe[0];
}

void	set_outfile(t_bonus *bonus, char **av, int ac)
{
	bonus->outfile_fd = open(av[ac - 1], O_RDWR | O_APPEND | O_CREAT, 0777);
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
