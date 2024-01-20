/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:41:50 by skanna            #+#    #+#             */
/*   Updated: 2024/01/19 17:28:59 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include "../src/pipex.h"

typedef struct s_pipex_bonus
{
	int		**pipe_fd;
	int		cmd_count;
	int		here_doc;
	int		input_here;
	pid_t	*pids;
}	t_pipex_bonus;

typedef struct s_lst
{
	char			*content;
	struct s_lst	*next;
}					t_lst;

# define ERROR_INIT "Unable to initialize pipex structure for the bonus part"

typedef enum e_error_code_bonus
{
	EC_INIT,
}	t_error_code_bonus;

int		ft_strcmp(const char *s1, const char *s2);
void	init_here_doc(t_pipex_bonus *b_struc, char **av);
char	*get_next_line(int fd);
void	free_all(t_lst **lst);
t_lst	*new_elem(char *buf, int bytes);
int		check_line(t_lst *lst, char c, int read_bytes);
void	check_rest(t_lst **lst, int i);

#endif