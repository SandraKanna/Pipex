/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:35 by skanna            #+#    #+#             */
/*   Updated: 2024/02/06 12:03:28 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Includes/pipex.h"

void	second_cmd(int *fd, int ac, char **av, char **envp)
{
	int		out_file;

	close(fd[1]);
	out_file = open(av[ac - 1], O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (out_file < 0)
		error_handling(EC_OUT);
	dup2(out_file, 1);
	close (out_file);
	dup2(fd[0], 0);
	close(fd[0]);
	if (!execute(av, envp, ac - 2))
		error_handling(EC_EXECVE);
}

void	first_cmd(int *fd, int ac, char **av, char **envp)
{
	int		in_file;

	close(fd[0]);
	in_file = open(av[ac - 4], O_RDONLY);
	if (in_file < 0)
		error_handling(EC_IN);
	dup2(in_file, 0);
	close (in_file);
	dup2(fd[1], 1);
	close(fd[1]);
	if (!execute(av, envp, ac - 3))
		error_handling(EC_EXECVE);
}

int	main(int argc, char **argv, char **envp)
{
	int		fd[2];
	pid_t	p_id;

	if (argc != 5)
		error_handling(EC_ARGS);
	if (pipe(fd) < 0)
		error_handling(EC_PIPE);
	p_id = fork();
	if (p_id < 0)
		error_handling(EC_FORK);
	else if (p_id == 0)
		first_cmd(fd, argc, argv, envp);
	else
	{
		close(fd[1]);
		second_cmd(fd, argc, argv, envp);
		if (waitpid(p_id, NULL, 0) == -1)
			error_handling(EC_WAIT);
		close(fd[0]);
	}
	return (0);
}
