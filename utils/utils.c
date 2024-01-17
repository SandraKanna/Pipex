/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:48 by skanna            #+#    #+#             */
/*   Updated: 2024/01/17 14:08:10 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pipex.h"

int	ft_strnstr(const char *big, const char *little, int len)
{
	int	i;

	i = 0;
	if (little[0] == '\0')
		return (0);
	while ((little[i]) && (big[i]) && i < len)
	{
		if (little[i] != big[i])
			return (0);
		i++;
	}
	if (little[i] == '\0')
		return (1);
	else
		return (0);
}

int	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *s)
{
	char	*dup;
	int		len_s;
	int		i;

	i = 0;
	len_s = ft_strlen(s);
	dup = malloc((len_s + 1) * sizeof(const char));
	if (dup == NULL)
		return (NULL);
	while (s[i])
	{
		dup[i] = ((const char *)s)[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*get_path_var(char **envp)
{
	int		i;
	int		check;
	char	*path;

	i = 0;
	check = 0;
	path = NULL;
	while (envp[i] && check != 1)
	{
		if (ft_strnstr((const char *)envp[i], "PATH=", 5))
		{
			path = ft_strdup((const char *)envp[i] + 5);
			break ;
		}
		i++;
	}
	if (path == NULL)
	{	
		perror("ERROR: PATH environment variable not found\n");
		exit (EXIT_FAILURE);
	}
	return (path);
}
