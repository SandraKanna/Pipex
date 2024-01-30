/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 15:46:23 by skanna            #+#    #+#             */
/*   Updated: 2024/01/30 16:17:33 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pipex.h"

char	*ft_empty(void)
{
	char	*str;

	str = malloc(1);
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	return (str);
}

void	free_cmds(char *full_cmd, char **cmd_args, char *path)
{
	int	i;

	i = 0;
	if (full_cmd)
		free(full_cmd);
	if (cmd_args)
	{
		while (cmd_args[i])
			free(cmd_args[i++]);//double free?
		free(cmd_args);
	}
	if (path)
		free(path);
}
