/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_for_parse.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/26 18:37:04 by skanna            #+#    #+#             */
/*   Updated: 2024/02/01 18:09:01 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/pipex.h"
//#include "../libft_v2/original_src/libft.h"

static int	count_args(char const *s, char c, int q)
{
	size_t	i;
	size_t	args;

	i = 0;
	args = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (q && (s[i] == 34 || s[i] == 39))
		{
			i++;
			args++;
			while (s[i] != 34 && s[i] != 39)
				i++;
			i++;
		}
		else
		{
			args++;
			while (s[i] != c && s[i])
				i++;
		}
	}
	return (args);
}

static int	has_quotes(char const *str)
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

static int	arg_len(char const *str, char c)
{
	int		i;
	int		len;
	int		in_quotes;
	char	q;

	i = 0;
	len = 0;
	in_quotes = 0;
	q = 0;
	while (str[i] && (in_quotes || str[i] != c))
	{
		if ((str[i] == 34 || str[i] == 39) && !in_quotes)
		{
			in_quotes = 1;
			q = str[i];
		}
		else if (str[i] == q && in_quotes)
			in_quotes = 0;
		i++;
		len++;
	}
	return (len);
}

static char	**fill_args(char **split, char const *s, size_t args, char c)
{
	size_t	h;
	size_t	len_args;

	h = 0;
	while (s && h < args)
	{
		len_args = 0;
		while (*s == c)
			s++;
		len_args = arg_len(s, c);
		if (*s == 34 || *s == 39)
		{
			s++;
			len_args -= 2;
		}
		split[h] = ft_substr(s, 0, len_args);
		if (!split[h])
		{
			free_split(split);
			return (NULL);
		}
		s += len_args;
		h++;
	}
	return (split);
}

char	**split_for_parse(char const *s, char c)
{
	size_t	args;
	char	**split;
	int		valid_quotes;

	if (!s)
		return (NULL);
	valid_quotes = has_quotes(s);
	if (valid_quotes < 0)
		return (NULL);
	args = count_args(s, c, valid_quotes);
	split = malloc ((args + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	split[args] = 0;
	split = fill_args(split, s, args, c);
	return (split);
}

/*int main(void)
{
	char *s = "sed    's/And/But/'";
	char c = 32;
	char **split;
	int	i;

	i = 0;
	printf("ft_split\n");
	printf("String to be copied: %s\n", s);
	split = ft_split(s, c);
	while (split[i])
	{
		printf("String %d: |%s|\n", i, split[i]);
		free (split[i]);
		i++;
		printf("\n");
	}
	free (split);
	printf("\n");
	return(0);
}*/
