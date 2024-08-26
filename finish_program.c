/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_program.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:12:41 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/26 15:40:24 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(t_pipex *info, char *str)
{
	perror(str);
	free_all(info);
	exit(1);
}

void	message_error(t_pipex *info, char *str)
{
	write(2, str, ft_strlen(str));
	free_all(info);
	exit(1);
}

void	return_status(t_pipex *info)
{
	free_all(info);
	if (WIFEXITED(info->status))
		exit(WEXITSTATUS(info->status));
}

void	free_all(t_pipex *info)
{
	if (info == NULL)
		return ;
	if (info->cmd1_path != NULL)
		free(info->cmd1_path);
	if (info->cmd2_path != NULL)
		free(info->cmd2_path);
	if (info->cmd1_splited != NULL)
		free_splited_arrays(info->cmd1_splited);
	if (info->cmd2_splited != NULL)
		free_splited_arrays(info->cmd2_splited);
	if (info->splited_path_envp != NULL)
		free_splited_arrays(info->splited_path_envp);
}

void	free_splited_arrays(char **strs)
{
	int	i;

	i = 0;
	while (strs[i] != NULL)
	{
		ft_printf("%d\n", i); //
		free(strs[i]);
		i++;
	}
	free(strs);
}
