/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 13:29:47 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/22 10:47:24 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*join_path(char *env, char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(env, "/");
	if (tmp == NULL)
		message_error("Unexpected Error\n");
	res = ft_strjoin(tmp, cmd);
	free(tmp);
	if (res == NULL)
		message_error("Unexpected Error\n");
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

void	free_two_dimensional_arrays(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_all(t_pipex *info)
{
	if (info == NULL)
		return ;
	if (info->splited_path_envp != NULL)
		free_two_dimensional_arrays(info->splited_path_envp);
}
