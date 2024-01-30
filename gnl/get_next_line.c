/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:12:39 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 13:08:19 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pipex.h"
#include "../bonus/bonus.h"

static void	free_all(t_lst **lst)
{
	t_lst	*temp;

	if (!*lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		free((*lst)->content);
		free(*lst);
		*lst = temp;
	}
}

static char	*get_line(t_lst **lst, char *line)
{
	t_lst	*temp;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (*lst && (*lst)->content[i] != '\n')
	{
		line[j++] = (*lst)->content[i++];
		if ((*lst)->content[i] == '\0')
		{
			temp = (*lst)->next;
			free ((*lst)->content);
			free (*lst);
			*lst = temp;
			i = 0;
		}
	}
	if ((*lst) && (*lst)->content[i++] == '\n')
		line[j] = '\n';
	check_rest(lst, i);
	return (line);
}

static int	save_in_lst(char *buffer, t_lst **lst, int read_bytes)
{
	t_lst	*temp;
	t_lst	*new;

	new = new_elem(buffer, read_bytes);
	if (!new)
		return (-1);
	if (!*lst)
		*lst = new;
	else
	{
		temp = *lst;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = new;
	}
	return (check_line(*lst, '\n', read_bytes));
}

static int	check_read(int fd, t_lst **lst, int buffer_size)
{
	int		b_read;
	int		len_line;
	char	*buffer;

	len_line = 0;
	buffer = malloc (buffer_size + 1);
	if (!buffer)
		return (-1);
	while (1)
	{
		b_read = read(fd, buffer, buffer_size);
		if (b_read < 0 || (b_read == 0 && *lst == NULL))
			return (free(buffer), -1);
		if (b_read == 0 && *lst != NULL)
			return (free(buffer), check_line(*lst, '\n', b_read));
		if (b_read > 0)
		{
			len_line = save_in_lst(buffer, lst, b_read);
			if (len_line == -1)
				return (free (buffer), -1);
			if (len_line > 0)
				return (free (buffer), len_line);
		}
	}
}

char	*get_next_line(int fd)
{
	static t_lst	*lst[1024];
	char			*line;
	int				len_line;
	int				buffer_size;

	buffer_size = 1024;
	if (fd < 0 || (buffer_size <= 0))
		return (NULL);
	len_line = check_read(fd, &(lst[fd]), buffer_size);
	if (len_line < 0)
	{
		free_all (&(lst[fd]));
		return (NULL);
	}
	line = malloc(sizeof(char) * (len_line + 1));
	if (!line)
	{
		free_all (&(lst[fd]));
		return (NULL);
	}
	line[len_line] = '\0';
	get_line(&(lst[fd]), line);
	return (line);
}
