#include "../src/pipex.h"

/*static int is_quote(char *arg) 
{
    // Check if arg starts or ends with a quote
    
    return 0;
}

static int find_end_quote(char **args, int start) 
{
    // Find the index of the end quote
    return 0;
}*/

char *merge_args(char **command_args, int start, int end) 
{
    char *merged_arg;
    char *temp_arg;
    int i = start;

	merged_arg = ft_strdup("");
    while (i <= end) 
	{
        temp_arg = ft_strjoin(merged_arg, command_args[i]);
        free(merged_arg);
        merged_arg = temp_arg;
        i++;
    }
    return (merged_arg);
}

/*char	**parse_cmd(char *command_str)
{
	char	**split_command;
	char	**final_command = NULL;
	int		start;
	int		end;
	int		i;

	i = 0;
	split_command = ft_split((char const *)command_str, ' ');
	while (split_command[i])
	{
		if (is_quote(split_command[i])) 
		{
			start = i;
			end = find_end_quote(split_command, start);
			final_command[i] = merge_args(split_command, start, end);
			i = end;  // Skip to the end of the quoted section
		}
		else
			final_command[i] = ft_strdup(split_command[i]);
		i++;
	}
	final_command[i] = NULL;
	return (final_command);
}*/

char	*parse_path(char *command, char **envp)
{
	char	*path_var;
	char	**split_path;
	char	*partial_path;
	char	*full_path;
	int		i;

	path_var = get_path_var(envp);
	if (path_var == NULL)
		error_handling(ERROR_PATH);
	split_path = ft_split((char const *)path_var, ':');
	i = 0;
	while (split_path[i])
	{
		partial_path = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(partial_path, command);
		free(partial_path);
		if (access(full_path, F_OK) == 0)
		{
			i = 0;
			while (split_path[i])
				free(split_path[i++]);
			free(split_path);
			return (full_path);
		}
		i++;
	}
	return (NULL);
}
