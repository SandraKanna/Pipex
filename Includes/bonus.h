/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:41:50 by skanna            #+#    #+#             */
/*   Updated: 2024/02/05 19:04:05 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include "pipex.h"
# include "../libft_v2/gnl_src/get_next_line.h"

typedef struct s_pipex_bonus
{
	int		**pipe_fd;
	int		cmd_count;
	pid_t	*pids;
	int		here_doc;
	int		infile_fd;
	int		outfile_fd;
}	t_bonus;

void	read_here_doc(t_bonus *bonus, char **av);
void	set_infile(t_bonus *bonus, char **av);
void	set_outfile(t_bonus *bonus, char **av, int ac);
void	close_fds(t_bonus *bonus, int current_cmd);
void	setup_n_exec(t_bonus *bonus, char **av, char **env, int ac);
void	create_pipes(t_bonus *bonus);

#endif