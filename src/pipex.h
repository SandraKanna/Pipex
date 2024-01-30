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

int		ft_strnstr(const char *big, const char *little, int len);
int		ft_strlen(const char *str);
char	*ft_empty(void);
char	*ft_strdup(const char *s);
char	*get_path_var(char **envp);
int		ft_strcmp(const char *s1, const char *s2);
int		check_abs_path(char *cmd);
char	*parse_cmd(char *path_var, char *command);
int		has_quotes(char const *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	**free_split(char **split);
char	**ft_split(char const *s, char c);
int		error_handling(t_error_code error_code);
void	free_cmds(char *full_cmd, char **cmd_args, char *path);

#endif