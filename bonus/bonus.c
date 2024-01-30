/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 16:43:27 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 15:04:11 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"

//https://github.com/mcombeau/pipex


//void	bonus_child(t_pipex_bonus *bonus, char **av, char **env, int i)

void	execute(char **av, char **env, int index)
{
	char	*cmd1_path;
	char	**cmd1_args;
	char	*path_var;


	
void	execute(char **av, char **env, int index)
{
	char	*cmd1_path;
	char	**cmd1_args;
	char	*path_var;


	cmd1_args = ft_split((const char *)av[index], ' ');
	//para err 8: agregar sh al comando ej: sh ./script.sh, buscabdo si len arg >3 y los ultimos chars son .sh => join sh + cmd
	path_var = get_path_var(env);
	cmd1_path = parse_path(path_var, cmd1_args[0]);

}

void	create_children(t_pipex_bonus *bonus, char **av, char **env, int hd)
{
	int		i;
	int		index;

	i = 0;
	index = 2;
	if (bonus->here_doc)
		index += 1;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
			execute (av, env, index);
		i++;
	}
}
	//para err 8: agregar sh al comando ej: sh ./script.sh, buscabdo si len arg >3 y los ultimos chars son .sh => join sh + cmd
	path_var = get_path_var(env);
	cmd1_path = parse_path(path_var, cmd1_args[0]);

}

/*void	create_children(t_pipex_bonus *bonus, char **av, char **env, int hd)
{
	int		i;
	int		index;

	i = 0;
	index = 2;
	if (bonus->here_doc)
		index += 1;
	while (i < bonus->cmd_count)
	{
		bonus->pids[i] = fork();
		if (bonus->pids[i] < 0)
			error_handling(EC_FORK);
		else if (bonus->pids[i] == 0)
			execute (av, env, index);
		i++;
	}
}*/

void	create_pipes(t_pipex_bonus *bonus)
{
	int	i;

	i = 0;
	while (i < bonus->cmd_count - 1)
	{
		if (pipe(bonus->pipe_fd[i]) < 0)
			error_handling(EC_PIPE);
		i++;
	}
}

void	init_struct(t_pipex_bonus *bonus, int ac, char **av, int here)
{
	int	i;

	i = 0;
	if (here)
		bonus->cmd_count = ac - 4;
	else
		bonus->cmd_count = ac - 3;
	bonus->pipe_fd = malloc (sizeof(int *) * (bonus->cmd_count - 1));
	bonus->pids = malloc (sizeof(pid_t) * (bonus->cmd_count));
	if (!bonus->pipe_fd || !bonus->pids)
		error_handling(EC_MEM);
	while (i < bonus->cmd_count - 1)
	{
		bonus->pipe_fd[i] = malloc (sizeof(int) * 2);
		if (!bonus->pipe_fd[i])
			error_handling(EC_PIPE);
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipex_bonus	bonus;
	int				i;
	int				wstatus;

	if (ac < 5)
		error_handling(EC_ARGS);
	bonus.here_doc = (ft_strcmp(av[1], "here_doc") == 0);
	init_struct(&bonus, ac, av, bonus.here_doc);
	create_pipes(&bonus);
	set_infile(&bonus, av);
	set_outfile(&bonus, av, ac);
	create_children(&bonus, av, envp, bonus.here_doc);
	i = 0;
	while (i < bonus.cmd_count)
	{
		if (waitpid(bonus.pids[i], &wstatus, 0) == -1)
			error_handling(EC_WAIT);
		i++;
	}
	return (0);
}
