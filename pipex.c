/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:32:57 by hnakayam          #+#    #+#             */
/*   Updated: 2024/09/01 20:20:02 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_cmd_is_empty_or_space(t_pipex *info, char *cmd)
{
	int	i;

	if (cmd[0] == '\0')
	{
		ft_printf("Command '' not found\n");
		free_all_exit(info, 127);
	}
	i = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] != ' ')
			return ;
		i++;
	}
	ft_printf("%s: command not found\n", cmd);
	free_all_exit(info, 127);
}

void	child_one(t_pipex *info, char **argv, char **environ)
{
	close(info->fds[0]);
	info->fd_in = open(argv[1], O_RDONLY);
	if (info->fd_in < 0)
	{
		close(info->fds[1]);
		ft_printf("bash: %s: %s\n", argv[1], strerror(errno));
		free_all_exit(info, 1);
	}
	check_cmd_is_empty_or_space(info, argv[2]);
	info->cmd1_splited = ft_split(argv[2], ' ');
	if (info->cmd1_splited == NULL)
		message_error(info, "Unxpected Error\n");
	info->cmd1_path = get_path_cmd(info, info->cmd1_splited[0], environ);
	if (dup2(info->fds[1], 1) < 0 || dup2(info->fd_in, 0) < 0)
	{
		close(info->fds[1]);
		close(info->fd_in);
		ft_printf("%s\n", strerror(errno));
		free_all_exit(info, 1);
	}
	close(info->fds[1]);
	close(info->fd_in);
	execve(info->cmd1_path, info->cmd1_splited, environ);
	error(info, "execve");
}

void	child_two(t_pipex *info, char **argv, char **environ)
{
	info->fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (info->fd_out < 0)
	{
		ft_printf("bash: %s: %s\n", argv[4], strerror(errno));
		free_all_exit(info, 1);
	}
	check_cmd_is_empty_or_space(info, argv[3]);
	info->cmd2_splited = ft_split(argv[3], ' ');
	if (info->cmd2_splited == NULL)
		message_error(info, "Unxpected Error\n");
	info->cmd2_path = get_path_cmd(info, info->cmd2_splited[0], environ);
	if (dup2(info->fds[0], 0) < 0 || dup2(info->fd_out, 1) < 0)
	{
		close(info->fds[0]);
		close(info->fd_out);
		ft_printf("%s\n", strerror(errno));
		free_all_exit(info, 1);
	}
	close(info->fds[0]);
	close(info->fd_out);
	execve(info->cmd2_path, info->cmd2_splited, environ);
	error(info, "execve");
}

void	split_envp_path(t_pipex *info, char **environ)
{
	char	*path_envp;

	path_envp = get_envp_path(environ);
	if (path_envp == NULL)
	{
		info->splited_path_envp = NULL;
	}
	else
	{
		info->splited_path_envp = ft_split(path_envp, ':');
	}
}

int	main(int argc, char *argv[], char **environ)
{
	t_pipex	info;

	if (argc != 5)
		message_error(NULL, "Invalid args\n");
	init_info(&info);
	split_envp_path(&info, environ);
	if (pipe(info.fds) < 0)
		error(&info, "pipe");
	info.child1 = fork();
	if (info.child1 == -1)
		error(&info, "fork");
	else if (info.child1 == 0)
		child_one(&info, argv, environ);
	else
		close(info.fds[1]);
	info.child2 = fork();
	if (info.child1 == -1)
		error(&info, "fork");
	else if (info.child2 == 0)
		child_two(&info, argv, environ);
	waitpid(info.child1, &info.status, 0);
	waitpid(info.child2, &info.status, 0);
	close(info.fds[0]);
	close(info.fds[1]);
	return_status(&info);
}

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q pipex");
// }

// infile, outfileどちらも権限がない場合、"permission denied"が2回表示される // ok
// infileに権限がなく、outfileが存在しない場合、outfileが作成され、"permission denied"が表示される // ok
// dup error ' == -1 ' // oumimoun // ok
// cmdに実行権限がないときに"No such file or directory"ではなく"Permission denied"を表示する // ok
// コマンドに実行権限がないとき(F_OK == 0 && X_OK == -1)は"Permission denied" // ok
// cmdに絶対パスを渡されたときの処理 // ok
// 絶対パスでコマンドを渡されたときのテスト
// permission 0644 // ok
// std err (ft_printf) // ok
// unset PATH // ok
// 終了ステータス
	// bashで確かめながら
// "" or " " as command
// splitなどでNULLが返ってきたときの処理
// leaks free() (after ex_cm2)
// bash
// unset PATH
// ls
// ./ls でなくても実行ファイル'ls'が実行される

// change function name (exec_cmd2 -> child_two exc.) // ok
// strchr -> ft_strchr // ok
//	nm -u ./pipex // ok
// mistake of spelling // ok
// 	diretory -> directory (no_envp func) // ok
// leaks // ok
// valgrind --leak-check=full --show-leak-kinds=all 
//	--trace-children=yes ./your_program_file [args]
// makefile "CC = cc -g" -> "CC = cc" // ok
// excute binary file user creates when evnp is not found

// confirm the operation when cmd is "sleep"
