/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:56 by skanna            #+#    #+#             */
/*   Updated: 2024/01/18 17:28:59 by skanna           ###   ########.fr       */
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
char	*ft_strdup(const char *s);
char	*get_path_var(char **envp);
char	*parse_path(char *path_var, char *command);
int		has_quotes(char const *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	**ft_split(char const *s, char c);
int		error_handling(t_error_code error_code);

#endif