/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:50:21 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/22 11:54:19 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "./ft_printf/ft_printf.h"
# include "./libft/libft.h"

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	int		fds[2];
	pid_t	child1;
	pid_t	child2;
	int		status;
	char	**splited_path_envp;
	char	*cmd1_path;
	char	*cmd2_path;
	char	**cmd1_splited;
	char	**cmd2_splited;
}	t_pipex;

void	child_one(t_pipex *info, char **argv, char **environ);
void	exec_cmd2(t_pipex *info, char **argv, char **environ);
void	split_envp_path(t_pipex *info, char **environ);
char	*rest_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*get_envp_path(char **environ);
char	*join_path(char *env, char *cmd);
void	error_no_envp(t_pipex *info, char *cmd_without_op);
void	check_cmd_is_empty_or_space(char *cmd);
char	*search_excutable_file(char *file);
char	*get_path_cmd(t_pipex *info, char *cmd, char **environ);
void	error(char *str);
void	message_error(char *str);
char	*ft_strndup(char *cmd);
void	free_two_dimensional_arrays(char **strs);
void	free_all(t_pipex *info);
void	return_status(t_pipex *info);

#endif