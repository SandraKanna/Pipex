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

int	ft_strcmp(const char *s1, const char *s2)
{
	int				i;
	unsigned char	*string1;
	unsigned char	*string2;

	i = 0;
	string1 = (unsigned char *)s1;
	string2 = (unsigned char *)s2;
	while ((string1[i]))
	{
		if (string1[i] == string2[i])
			i++;
		else
			return (string1[i] - string2[i]);
	}
	return (string1[i] - string2[i]);
}

void	setup_io(t_pipex_bonus *b_struc, int i, int here, int input_here)
{

}

int	here_doc_exec(t_pipex_bonus *b_struc, char **av, char **env)
{
	int	i;
	int	input_here;

	i = 0;
	input_here = b_struc->input_here;
	while (i < b_struc->cmd_count)
	{
		b_struc->pids[i] = fork();
		if (b_struc->pids[i] < 0)
			error_handling(EC_FORK);
		else if (b_struc->pids[i] == 0)
		{
			
		}
	}
	i = 0;
	while (i < b_struc->cmd_count)
	{
		if (waitpid(b_struc->pids[i], NULL, 0) == -1)
			error_handling(EC_WAIT);
	}
}

void	init_here_doc(t_pipex_bonus *b_struc, char **av)
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
		write(hd_pipe[1], line, ft_strlen(line));
		free(line);
	}
	close(hd_pipe[1]);
	b_struc->input_here = hd_pipe[0];
}


