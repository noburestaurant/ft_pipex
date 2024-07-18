/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 16:12:41 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/18 13:34:21 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(char *str)
{
	perror(str);
	exit(1);
}

void	message_error(char *str)
{
	write(2, str, ft_strlen(str));
	exit(1);
}
