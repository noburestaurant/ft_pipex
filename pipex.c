/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:53:56 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/17 16:26:51 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_pipex *info, char *cmd1, char **environ)
{
	// char	*cmd[3] = {"grep", "pipe", NULL};
	info->cmd1_splited = ft_split(cmd1, ' ');
	close(info->fds[0]);
	dup2(info->fds[1], 1);
	dup2(info->fd_in, 0);
	close(info->fds[1]);
	close(info->fd_in);
	// execve("/usr/bin/grep", cmd, environ);
	execve(info->cmd1_path, info->cmd1_splited, environ);
	error("execve");
}

void	exec_cmd2(t_pipex *info, char *cmd2, char **environ)
{
	// char *cmd2[3] = {"wc", "-l", NULL};
	info->cmd2_splited = ft_split(cmd2, ' ');
	dup2(info->fds[0], 0);
	close(info->fds[0]);
	dup2(info->fd_out, 1);
	close(info->fd_out);
	execve(info->cmd2_path, info->cmd2_splited, environ);
	error("execve");
}

void	file_open(t_pipex *info, char *file1, char *file2)
{
	info->fd_in = open(file1, O_RDONLY);
	if (info->fd_in < 0)
		error("open");
	info->fd_out = open(file2, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (info->fd_out < 0)
		error("open");
}

// void	init_info(t_pipex *info, char *cmd1, char *cmd2)
// {
// 	info->cmd1_splited = ft_split(cmd1);
// 	info->cmd2_splited = ft_split(cmd2);
// }

int	main(int argc, char *argv[], char **environ)
{
	t_pipex	info;

	if (argc != 5)
		exit(1);
	file_open(&info, argv[1], argv[4]);
	// init_info(&info, argv[2], argv[3]);
	info.cmd1_path = get_path_cmd(argv[2], environ);
	if (info.cmd1_path == NULL)
		error("cmd1"); //
	if (pipe(info.fds) < 0)
		error("pipe");
	info.child1 = fork();
	if (info.child1 == 0)
		child_one(&info, argv[2], environ);
	else
		close(info.fds[1]);
	waitpid(info.child1, &info.status, 0);
	info.cmd2_path = get_path_cmd(argv[3], environ);
	if (info.cmd2_path == NULL)
		error("cmd2"); //
	exec_cmd2(&info, argv[3], environ);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }

