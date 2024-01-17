/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:56 by skanna            #+#    #+#             */
/*   Updated: 2024/01/17 13:58:19 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>

# define ERROR_ARGS "Please enter the right parameters: file1 cmd1 cmd2 file2"
# define ERROR_PIPE "Cannot create pipe"
# define ERROR_FORK "Error creating child1 process with fork"
# define ERROR_WAIT "Unable to launch waitpid"
# define ERROR_INFILE "Unable to open in_file"
# define ERROR_OUTFILE "Unable to open or create out_file"
# define ERROR_PATH "ERROR: PATH environment variable not found"
# define ERROR_CMD "Command not found\n"
# define ERROR_EXECVE "Execve failure"

int		ft_strnstr(const char *big, const char *little, int len);
int		ft_strlen(const char *str);
char	*ft_strdup(const char *s);
char	*get_path_var(char **envp);
char	*parse_path(char *command, char **envp);
char *merge_args(char **command_args, int start, int end);
char	**parse_cmd(char *command_str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
int		error_handling(char *error_msg);

#endif