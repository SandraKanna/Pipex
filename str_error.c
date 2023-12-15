/*
	#include <string.h>
	char *strerror(int errnum);

Returns a pointer to a string that describes the error code passed 
in the argument errnum

ERRORS TO RETURN
	   EINVAL The value of errnum is not a valid error number.
	   ERANGE Insufficient storage was supplied to contain the error 
	   description string.

*/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int	main(void)
{
	int	fd;

	fd = open("nonexistent_file.txt", O_RDONLY);
	if (fd == -1)
	{
		if (errno == EBADF)
			printf("error 9 EBADF: %s\n", strerror(errno));
		else if (errno == ENOENT)
			printf("error 2 ENOENT: %s\n", strerror(errno));
		else if (errno == EACCES)
			printf("error 13 EACCES: %s\n", strerror(errno));
		else
			printf("other error: %s\n", strerror(errno));
	}
	else
		close(fd);
	return (0);
}
