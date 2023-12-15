/*
	#include <stdio.h>
	void perror(const char *s);

Produces a message on standard error describing the last error 
encountered during a call to a system or library function.
*/

#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
	int	file;

	file = open("nonexistent_file.txt", O_RDONLY);
	if (file == -1)
		perror("Error opening your f***ing file!");
	return (0);
}
