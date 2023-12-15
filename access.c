/*
	#include <unistd.h>
	int access(const char *pathname, int mode);

Checks whether the calling process can access the file pathname.
If pathname is a symbolic link, it is dereferenced.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	const char	*filename = "test2";

	if (access(filename, R_OK) == 0)
		printf("%s is readable.\n", filename);
	else
		perror("Read access error");
	if (access(filename, W_OK) == 0)
		printf("%s is writable.\n", filename);
	else
		perror("Write access error");
	if (access(filename, X_OK) == 0)
		printf("%s is executable.\n", filename);
	else
		perror("Execute access error");
	return (0);
}
