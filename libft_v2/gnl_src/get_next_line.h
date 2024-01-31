/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 15:44:09 by skanna            #+#    #+#             */
/*   Updated: 2023/11/27 13:42:10 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <stdlib.h>
# include <unistd.h>

typedef struct s_lst
{
	char			*content;
	struct s_lst	*next;
}					t_lst;

char	*get_next_line(int fd);
void	free_all(t_lst **lst);
t_lst	*new_elem(char *buf, int bytes);
int		check_line(t_lst *lst, char c, int read_bytes);
void	check_rest(t_lst **lst, int i);

#endif