/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 15:33:27 by hnakayam          #+#    #+#             */
/*   Updated: 2024/08/31 15:33:28 by hnakayam         ###   ########.fr       */
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
# include "./ft_printf_stderr/ft_printf.h"
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
void	child_two(t_pipex *info, char **argv, char **environ);
void	split_envp_path(t_pipex *info, char **environ);
char	*rest_strnstr(const char *haystack, const char *needle, size_t len);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
char	*get_envp_path(char **environ);
char	*join_path(t_pipex *info, char *env, char *cmd);
void	error_no_envp(t_pipex *info, char *cmd_without_op);
void	check_cmd_is_empty_or_space(t_pipex *info, char *cmd);
char	*search_excutable_file(t_pipex *info, char *file);
char	*get_path_cmd(t_pipex *info, char *cmd, char **environ);
void	error(t_pipex *info, char *str);
void	message_error(t_pipex *info, char *str);
char	*ft_strndup(char *cmd);
void	free_splited_arrays(char **strs);
void	free_all_exit(t_pipex *info, int return_status);
void	init_info(t_pipex *info);
void	return_status(t_pipex *info);

#endif
