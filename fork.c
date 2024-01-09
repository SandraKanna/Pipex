
#include <stdio.h>
#include <unistd.h>

int main() {
    pid_t child_pid;

    // Create a new process
    child_pid = fork();

    if (child_pid == -1) {
        // Error handling if fork fails
        perror("fork");
        return 1;
    }

    if (child_pid == 0) {
        // Code executed by the child process
        printf("Child process\n");
    } else {
        // Code executed by the parent process
        printf("Parent process, child PID: %d\n", child_pid);
    }

    return 0;
}
