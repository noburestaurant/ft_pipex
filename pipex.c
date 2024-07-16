/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:53:56 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/16 23:30:23 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_pipex *info, char **environ)
{
	char	*cmd[3] = {"grep", "pipe", NULL};
	close(info->fds[0]);
	dup2(info->fds[1], 1);
	dup2(info->fd_in, 0);
	close(info->fds[1]);
	close(info->fd_in);
	execve("/usr/bin/grep", cmd, environ);
	error("execve");
}

void	exec_cmd2(t_pipex *info, char **environ)
{
	char *cmd2[3] = {"wc", "-l", NULL};
	dup2(info->fds[0], 0);
	close(info->fds[0]);
	dup2(info->fd_out, 1);
	close(info->fd_out);
	execve("/usr/bin/wc", cmd2, environ);
	error("execve");
}

int	main(int argc, char *argv[], char **environ)
{
	t_pipex	info;

	if (argc != 3)
		exit(1);
	info.fd_in = open(argv[1], O_RDONLY);
	if (info.fd_in < 0)
		error("open");
	info.fd_out = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (info.fd_out < 0)
		error("open");
	if (pipe(info.fds) < 0)
		error("pipe");
	info.child1 = fork();
	if (info.child1 == 0)
		child_one(&info, environ);
	else
		close(info.fds[1]);
	waitpid(info.child1, &info.status, 0);
	exec_cmd2(&info, environ);
}
