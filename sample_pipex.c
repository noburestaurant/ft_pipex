#include "sample_pipex.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void child_one(t_pipex *info, char **environ)
{
    char *cmd[3] = {"grep", "pipe", NULL};
    close(info->fds[0]);
    dup2(info->fds[1], STDOUT_FILENO);
    close(info->fds[1]);
    dup2(info->fd_in, STDIN_FILENO);
    close(info->fd_in);
    execve("/usr/bin/grep", cmd, environ);
    error("execve");
}

void child_two(t_pipex *info, char **environ) {
    char *cmd[3] = {"wc", "-l", NULL};
    close(info->fds[1]);
    dup2(info->fds[0], STDIN_FILENO);
    close(info->fds[0]);
    dup2(info->fd_out, STDOUT_FILENO);
    close(info->fd_out);
    execve("/usr/bin/wc", cmd, environ);
    error("execve");
}

int main(int argc, char *argv[], char **environ) {
    t_pipex info;

    if (argc != 3)
        exit(1);
    info.fd_in = open(argv[1], O_RDONLY);
    if (info.fd_in < 0)
        error("open");
    info.fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0644);
    if (info.fd_out < 0)
        error("open");
    if (pipe(info.fds) < 0)
        error("pipe");

    info.child1 = fork();
    if (info.child1 < 0)
        error("fork");
    if (info.child1 == 0)
        child_one(&info, environ);

    info.child2 = fork();
    if (info.child2 < 0)
        error("fork");
    if (info.child2 == 0)
        child_two(&info, environ);

    close(info.fds[0]);
    close(info.fds[1]);
    close(info.fd_in);
    close(info.fd_out);

    waitpid(info.child1, &info.status, 0);
    waitpid(info.child2, &info.status, 0);

    return 0;
}
