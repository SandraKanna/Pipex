/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:56 by skanna            #+#    #+#             */
/*   Updated: 2024/02/01 18:15:59 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

# include "../libft_v2/original_src/libft.h"

# define ERROR_ARGS "Please enter the right parameters"
# define ERROR_PIPE "Unable to create pipe"
# define ERROR_FORK "Error creating child1 process"
# define ERROR_WAIT "Waitpid returned error"
# define ERROR_INPUT "Input error"
# define ERROR_OUTFILE "Outfile error"
# define ERROR_EXECVE "Command not found"
# define ERROR_MEM "Malloc error"
# define ERROR_INIT "Unable to initialize pipex structure for the bonus part"

typedef enum e_error_code
{
	EC_ARGS,
	EC_PIPE,
	EC_FORK,
	EC_WAIT,
	EC_IN,
	EC_OUT,
	EC_EXECVE,
	EC_MEM,
	EC_INIT,
}	t_error_code;

void	free_cmds(char *full_cmd, char **cmd_args, char *path);
int		error_handling(t_error_code error_code);
char	*get_path_var(char **envp);
int		absolute_path(char *cmd);
int		is_script(char *cmd);
char	**split_for_parse(char const *s, char c);
char	*parse_cmd(char *path_var, char *command);
int		execute(char **av, char **envp, int index);
void	second_cmd(int *fd, int ac, char **av, char **envp);
void	first_cmd(int *fd, int ac, char **av, char **envp);

#endif