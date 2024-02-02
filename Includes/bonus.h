/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:41:50 by skanna            #+#    #+#             */
/*   Updated: 2024/02/01 18:38:42 by skanna           ###   ########.fr       */
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
	int		here_doc;
	pid_t	*pids;
}	t_bonus;

// typedef struct s_list
// {
// 	char			*content;
// 	struct s_lst	*next;
// }					t_list;

void	create_pipes(t_bonus *bonus);
void	read_here_doc(t_bonus *bonus, char **av);
void	set_infile(t_bonus *bonus, char **av);
void	set_outfile(t_bonus *bonus, char **av, int ac);
void	fork_children(t_bonus *bonus, char **av, char **env);
void	exec_bonus(t_bonus *bonus, char **av, char **envp, int index);
void	init_struct(t_bonus *bonus, int ac, int here);

#endif