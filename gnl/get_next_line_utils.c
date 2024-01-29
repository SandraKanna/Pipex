/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/16 19:12:39 by skanna            #+#    #+#             */
/*   Updated: 2024/01/29 13:08:02 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../bonus/bonus.h"

t_lst	*new_elem(char *buf, int bytes)
{
	t_lst	*new_node;
	int		i;

	new_node = malloc(sizeof(t_lst));
	if (!new_node)
		return (NULL);
	new_node->content = malloc(sizeof(char) * (bytes + 1));
	if (!new_node->content)
	{
		free (new_node->content);
		free (new_node);
		return (NULL);
	}
	i = 0;
	while (i < bytes && buf[i])
	{
		new_node->content[i] = buf[i];
		i++;
	}
	new_node->content[bytes] = '\0';
	new_node->next = NULL;
	return (new_node);
}

int	check_line(t_lst *lst, char c, int read_bytes)
{
	int	i;
	int	len;

	len = 0;
	if (!lst)
		return (-1);
	while (lst != NULL)
	{
		i = 0;
		while (lst->content[i])
		{
			if (lst->content[i] == c)
			{
				len++;
				return (len);
			}
			i++;
			len++;
		}
		lst = lst->next;
	}
	if (read_bytes > 0)
		return (0);
	return (len);
}

void	check_rest(t_lst **lst, int i)
{
	t_lst	*temp;
	int		j;

	j = 0;
	if ((*lst) && (*lst)->content[i] == '\0')
	{
		temp = (*lst)->next;
		free ((*lst)->content);
		free(*lst);
		*lst = temp;
		return ;
	}
	while ((*lst) && (*lst)->content[i] != '\0')
		(*lst)->content[j++] = (*lst)->content[i++];
	if (*lst)
		(*lst)->content[j] = '\0';
	return ;
}
