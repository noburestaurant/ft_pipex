/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:50:21 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/16 19:03:44 by hnakayam         ###   ########.fr       */
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

typedef struct s_pipex
{
	int		fd_in;
	int		fd_out;
	int		fds[2];
	pid_t	child1;
	pid_t	child2;
	int		status;
	char	*cmd1_path;
	char	*cmd2_path;
}	t_pipex;

void	error(char *str);
// void error(const char *msg);

#endif