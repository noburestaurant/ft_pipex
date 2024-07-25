/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:55:00 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/25 20:47:06 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <string.h>

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


char	*join_path(char *env, char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(env, "/");
	if (tmp == NULL)
		message_error("Error\n");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	if (res == NULL)
		message_error("Error\n");
	return (res);
}

char	*ft_strndup(char *cmd)
{
	size_t	len;
	size_t	i;
	char	*res;

	len = 0;
	while (cmd[len] != ' ' && cmd[len] != '\0')
		len++;
	res = (char *)malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		res[i] = cmd[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*get_path_cmd(t_pipex *info, char *cmd, char **environ)
{
	char	*cmd_without_op;
	char	*path_cmd;
	int		i;

	cmd_without_op = ft_strndup(cmd); // if (check_absolute_path(cmd_without_op))
	if (cmd_without_op == NULL)
		message_error("Error\n");
	i = 0;
	while (info->splited_path_envp[i] != NULL)
	{
		path_cmd = join_path(info->splited_path_envp[i], cmd_without_op);
		if (!access(path_cmd, F_OK))
		{
			if (!access(path_cmd, X_OK)) // check_excute_right();
			{
				free(path_cmd);
				free(cmd_without_op);
				return (path_cmd);
			}
			ft_printf("bash: %s: %s\n", path_cmd, strerror(errno));
			free(cmd_without_op);
			free(path_cmd);
			exit(1);
		}
		free(path_cmd);
		i++;
	}
	free(cmd_without_op);
	return (NULL);
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
