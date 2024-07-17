/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnakayam <hnakayam@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:54:32 by hnakayam          #+#    #+#             */
/*   Updated: 2024/07/17 18:54:43 by hnakayam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*rest_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	temp;
	size_t	n;

	i = 0;
	n = 0;
	while (haystack[i] && n < len)
	{
		j = 0;
		temp = i;
		while (haystack[temp] == needle[j] && temp < len)
		{
			if (j >= len)
				break ;
			j++;
			temp++;
			if (needle[j] == '\0')
				return ((char *)&haystack[i]);
		}
		i++;
		n++;
	}
	return (NULL);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	if (needle[0] == '\0')
		return ((char *)haystack);
	if (len == 0)
		return (NULL);
	return (rest_strnstr(haystack, needle, len));
}

char	*nobu_strcpy(char *ans, char const *s, int start, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		ans[i] = s[start + i];
		i++;
	}
	ans[i] = '\0';
	return (ans);
}

void	all_free(char **ans, int i)
{
	while (i > 0)
	{
		i--;
		free(ans[i]);
	}
	free(ans);
}

int	count_words(char const *s, char c)
{
	int	count;
	int	flag;
	int	i;

	count = 0;
	flag = 0;
	i = 0;
	while (s[i])
	{
		if (flag == 0 && s[i] != c)
		{
			count++;
			flag = 1;
		}
		else if (flag == 1 && s[i] == c)
		{
			flag = 0;
		}
		i++;
	}
	return (count);
}

char	**rest_of_split(char const *s, char c, char **ans, int count)
{
	int	i;
	int	start;
	int	len;

	start = 0;
	i = 0;
	while (i < count)
	{
		while (s[start] == c)
			start++;
		len = 0;
		while (s[start + len] != c && s[start + len])
			len++;
		ans[i] = (char *)malloc(sizeof(char) * (len + 1));
		if (ans[i] == NULL)
		{
			all_free(ans, i);
			return (NULL);
		}
		nobu_strcpy(ans[i], s, start, len);
		i++;
		start += len;
	}
	ans[i] = NULL;
	return (ans);
}

char	**ft_split(char const *s, char c)
{
	char	**ans;
	int		count;

	count = count_words(s, c);
	ans = (char **)malloc(sizeof(char *) * (count + 1));
	if (ans == NULL)
		return (NULL);
	return (rest_of_split(s, c, ans, count));
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		total_len;
	char	*ans;
	int		i;

	total_len = (int)ft_strlen(s1) + (int)ft_strlen(s2) + 1;
	ans = (char *)malloc(sizeof(char) * total_len);
	if (ans == NULL)
		return (NULL);
	i = 0;
	while (*s1)
	{
		ans[i] = *s1;
		s1++;
		i++;
	}
	while (*s2)
	{
		ans[i] = *s2;
		s2++;
		i++;
	}
	ans[i] = '\0';
	return (ans);
}
