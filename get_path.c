/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:25:47 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/31 15:28:40 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_envp_path(char **environ)
{
	int		i;
	char	*res;

	i = 0;
	while (environ[i])
	{
		res = ft_strnstr(environ[i], "PATH", 4);
		if (res)
			return (res + 5);
		i++;
	}
	return (NULL);
}

char	*search_excutable_file(t_pipex *info, char *file)
{
	if (ft_strchr(file, '.') || ft_strchr(file, '/'))
	{
		if (!access(file, F_OK))
		{
			if (!access(file, X_OK))
			{
				return (file);
			}
			ft_printf("bash: %s: %s\n", file, strerror(errno));
			free_all_exit(info, 126);
		}
		else
		{
			ft_printf("bash: %s: %s\n", file, strerror(errno));
			free_all_exit(info, 127);
		}
	}
	return (NULL);
}

void	error_no_envp(t_pipex *info, char *cmd_without_op)
{
	ft_printf("bash: %s: No such file or directory\n", cmd_without_op);
	free(cmd_without_op);
	exit(127);
}

char	*search_cmd(t_pipex *info, char *cmd_without_op)
{
	char	*path_cmd;
	int		i;

	i = 0;
	if (info->splited_path_envp == NULL)
		error_no_envp(info, cmd_without_op);
	while (info->splited_path_envp[i] != NULL)
	{
		path_cmd = join_path(info, info->splited_path_envp[i], cmd_without_op);
		if (!access(path_cmd, F_OK))
		{
			if (!access(path_cmd, X_OK))
			{
				return (path_cmd);
			}
			ft_printf("bash: %s: %s\n", path_cmd, strerror(errno));
			free(cmd_without_op);
			free(path_cmd);
			free_all_exit(info, 126);
		}
		free(path_cmd);
		i++;
	}
	return (NULL);
}

char	*get_path_cmd(t_pipex *info, char *cmd, char **environ)
{
	char	*cmd_without_op;
	char	*path_cmd;

	if (search_excutable_file(info, cmd))
		return (cmd);
	cmd_without_op = ft_strndup(cmd);
	if (cmd_without_op == NULL)
		message_error(info, "Unxpected Error\n");
	path_cmd = search_cmd(info, cmd_without_op);
	if (path_cmd == NULL)
	{
		ft_printf("%s: command not found\n", cmd_without_op);
		free(cmd_without_op);
		free_all_exit(info, 127);
	}
	free(cmd_without_op);
	return (path_cmd);
}

// int	main(int argc, char *argv[], char **environ)
// {
// 	char	*cmd_path;

// 	cmd_path = get_path_cmd(argv[argc - 1], environ); // argv[2]
// 	if (cmd_path == NULL)
// 		printf("error\n");
// 	else
// 		printf("cmd_path = %s\n", cmd_path);
// 	return (0);
// }

// __attribute__((destructor))
// static void destructor() {
//     system("leaks -q a.out");
// }
