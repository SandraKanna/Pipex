#include "../src/pipex.h"

char	*ft_vide(void)
{
	char	*str;

	str = malloc(1);
	if (str == NULL)
		return (NULL);
	str[0] = '\0';
	return (str);
}

void	free_args(char *full_cmd, char **cmd_args, char *path)
{
	int	i;

	i = 0;
	if (full_cmd)
		free(full_cmd);
	if (cmd_args)
	{
		while (cmd_args[i])
			free(cmd_args[i++]);
		free(cmd_args);
	}
	if (path)
		free(path);
}