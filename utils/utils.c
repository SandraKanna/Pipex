/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 11:33:48 by skanna            #+#    #+#             */
/*   Updated: 2024/01/18 18:25:40 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../src/pipex.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*joint;
	int		len_s1;
	int		len_s2;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 ||!s2)
		return (NULL);
	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	joint = malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (joint == NULL)
		return (NULL);
	while (s1[j])
		joint [i++] = ((char *)s1)[j++];
	j = 0;
	while (s2[j])
		joint[i++] = ((char *)s2)[j++];
	joint[i] = '\0';
	return (joint);
}

int	has_quotes(char const *str)
{
	int	i;
	int	quotes;

	i = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == 34 || str[i] == 39)
			quotes++;
		i++;
	}
	if (quotes % 2 == 0)
		return (1);
	else if (quotes == 0)
		return (0);
	return (-1);
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
	return (path);
}

int	error_handling(char *error_msg)
{
	perror(error_msg);
	exit (EXIT_FAILURE);
}
