/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:53:56 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/18 21:47:29 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_one(t_pipex *info, char *cmd1, char **environ)
{
	info->cmd1_splited = ft_split(cmd1, ' ');
	close(info->fds[0]);
	dup2(info->fds[1], 1);
	dup2(info->fd_in, 0);
	close(info->fds[1]);
	close(info->fd_in);
	execve(info->cmd1_path, info->cmd1_splited, environ);
	error("execve");
}

void	exec_cmd2(t_pipex *info, char *cmd2, char **environ)
{
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
		error(file1);
	info->fd_out = open(file2, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (info->fd_out < 0)
		error(file2);
}

void	split_envp_path(t_pipex *info, char **environ)
{
	char	*path_envp;

	path_envp = get_envp_path(environ);
	if (path_envp == NULL)
		error("no $PATH"); // message incorrect
	info->splited_path_envp = ft_split(path_envp, ':');
}

int	main(int argc, char *argv[], char **environ)
{
	t_pipex	info;

	if (argc != 5)
		message_error("Invalid args\n"); // message incorrect?
	file_open(&info, argv[1], argv[4]);
	split_envp_path(&info, environ);
	info.cmd1_path = get_path_cmd(&info, argv[2], environ);
	if (info.cmd1_path == NULL)
		error(ft_strndup(argv[2])); // leaks??
	if (pipe(info.fds) < 0)
		error("pipe");
	info.child1 = fork();
	if (info.child1 == -1)
		error("fork");
	else if (info.child1 == 0)
		child_one(&info, argv[2], environ);
	else
		close(info.fds[1]);
	waitpid(info.child1, &info.status, 0);
	info.cmd2_path = get_path_cmd(&info, argv[3], environ);
	if (info.cmd2_path == NULL)
		error(ft_strndup(argv[3]));
	exec_cmd2(&info, argv[3], environ);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q pipex");
// }

// cmdに絶対パスを渡されたときの処理
// infile, outfileどちらも権限がない場合、"permission denied"が2回表示される
// infileに権限がなく、outfileが存在しない場合、outfileが作成され、"permission denied"が表示される
// gab
