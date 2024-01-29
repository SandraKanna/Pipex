/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus-utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 14:12:24 by skanna           ###   ########.fr       */
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

int	here_doc_exec(t_pipex_bonus *bonus, char **av, char **env)
{
	int	i;
	int	input_here;

	i = 0;
	input_here = bonus->input_hd;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
		{
			
		}
	}
	i = 0;
	while (i < bonus->cmd_count)
	{
		if (waitpid(bonus->pids[i], NULL, 0) == -1)
			error_handling(EC_WAIT);
	}
}

void	init_here_doc(t_pipex_bonus **bonus, char **av)
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
	(*bonus)->input_hd = hd_pipe[0];
}


