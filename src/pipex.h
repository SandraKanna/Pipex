/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:56 by skanna            #+#    #+#             */
/*   Updated: 2024/01/30 16:04:51 by skanna           ###   ########.fr       */
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
# define ERROR_INFILE "Unable to open in_file"
# define ERROR_OUTFILE "Unable to open or create out_file"
# define ERROR_EXECVE "Command not found"
# define ERROR_MEM "Unable to allocate memory"

typedef enum e_error_code
{
	EC_ARGS,
	EC_PIPE,
	EC_FORK,
	EC_WAIT,
	EC_INFILE,
	EC_OUTFILE,
	EC_EXECVE,
	EC_MEM,
}	t_error_code;

void	free_cmds(char *full_cmd, char **cmd_args, char *path);
int		error_handling(t_error_code error_code);
char	*get_path_var(char **envp);
int		absolute_path(char *cmd);
int		is_script(char *cmd);
char	**split_for_parse(char const *s, char c);
char	*parse_cmd(char *path_var, char *command);

#endif