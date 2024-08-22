/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 19:02:14 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/21 20:21:45hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	check_cmd_is_empty_or_space(char *cmd)
{
	int	i;

	if (cmd[0] == '\0')
	{
		ft_printf("Command '' not found\n");
		exit(127);
	}
	i = 0;
	while (cmd[i] != '\0')
	{
		if (cmd[i] != ' ')
			return ;
		i++;
	}
	ft_printf("%s: command not found\n", cmd);
	exit(127);
}

void	child_one(t_pipex *info, char **argv, char **environ)
{
	close(info->fds[0]);
	info->fd_in = open(argv[1], O_RDONLY);
	if (info->fd_in < 0)
	{
		close(info->fds[1]);
		ft_printf("bash: %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}
	check_cmd_is_empty_or_space(argv[2]);
	info->cmd1_splited = ft_split(argv[2], ' ');
	info->cmd1_path = get_path_cmd(info, info->cmd1_splited[0], environ);
	if (dup2(info->fds[1], 1) < 0 || dup2(info->fd_in, 0) < 0)
	{
		close(info->fds[1]);
		close(info->fd_in);
		ft_printf("%s\n", strerror(errno));
		exit(1);
	}
	close(info->fds[1]);
	close(info->fd_in);
	execve(info->cmd1_path, info->cmd1_splited, environ);
	error("execve");
}

void	exec_cmd2(t_pipex *info, char **argv, char **environ)
{
	info->fd_out = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (info->fd_out < 0)
	{
		ft_printf("bash: %s: %s\n", argv[4], strerror(errno));
		exit(1);
	}
	check_cmd_is_empty_or_space(argv[3]);
	info->cmd2_splited = ft_split(argv[3], ' ');
	info->cmd2_path = get_path_cmd(info, info->cmd2_splited[0], environ);
	if (dup2(info->fds[0], 0) < 0 || dup2(info->fd_out, 1) < 0)
	{
		close(info->fds[0]);
		close(info->fd_out);
		ft_printf("%s\n", strerror(errno));
		exit(1);
	}
	close(info->fds[0]);
	close(info->fd_out);
	execve(info->cmd2_path, info->cmd2_splited, environ);
	error("execve");
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
		message_error("Invalid args\n");
	split_envp_path(&info, environ);
	if (pipe(info.fds) < 0)
		error("pipe");
	info.child1 = fork();
	if (info.child1 == -1)
		error("fork");
	else if (info.child1 == 0)
		child_one(&info, argv, environ);
	else
		close(info.fds[1]);
	waitpid(info.child1, &info.status, 0);
	info.child2 = fork();
	if (info.child1 == -1)
		error("fork");
	else if (info.child2 == 0)
		exec_cmd2(&info, argv, environ);
	waitpid(info.child2, &info.status, 0);
	close(info.fds[0]);
	close(info.fds[1]);
	if (WIFEXITED(info.status))
		exit(WEXITSTATUS(info.status));
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
// 
// leaks free() (after ex_cm2)

// bash
// unset PATH
// ls
// ./ls でなくても実行ファイル'ls'が実行される

// *bash 
// < aainfile cat | grep char > outfile
// bash: aainfile: Permission denied
// *my_pipex // ok
// ./pipex "aainfile" "cat" "grep char" "outfile"
// bash: aainfile: Permission denied
// *bash
// < empty cat | grep char > outfile
// bash: empty: No such file or directory
// *my_pipex // ok
// ./pipex "empty" "cat" "grep char" "outfile"
// bash: empty: No such file or directory
// *bash
// < empty ccat | ggrep char > outfile
// bash: empty: No such file or directory
// bash: ggrep: command not found
// *my_pipex // ok
// ./pipex "empty" "ccat" "ggrep char" "outfile"
// bash: empty: No such file or directory
// bash: ggrep: command not found
// *bash
// < infile testecho | grep char > outfile
// bash: /Users/hnakayam/mybin/testecho: Permission denied
// *my_pipex
// ./pipex "testecho" "cat" "grep char" "outfile"
// bash: testecho: No such file or directory
