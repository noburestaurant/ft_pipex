#include "pipex.h"
#include <string.h>

char 	*get_envp_path(char **environ)
{
	int		i;
	char	*res;

	i = 0;
	while (environ[i])
	{
		res = strnstr(environ[i], "PATH", 4);
		if (res)
			return (res);
		i++;
	}
	return (NULL);
}

static char	*nobu_strcpy(char *ans, char const *s, int start, int len)
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

static void	all_free(char **ans, int i)
{
	while (i > 0)
	{
		i--;
		free(ans[i]);
	}
	free(ans);
}

static int	count_words(char const *s, char c)
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

static char	**rest_of_split(char const *s, char c, char **ans, int count)
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

char	*join_path(char *env, char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(env, "/");
	if (tmp == NULL)
		return (NULL);
	res = ft_strjoin(tmp, cmd);
	if (res == NULL)
		return (NULL);
	free(tmp);
	return (res);
}

char	*get_path_cmd(char *cmd, char **environ)
{
	char	**splited_cmd;
	char	*path_envp;
	char	**splited_path_envp;
	char	*path_cmd;
	int		i;

	splited_cmd = ft_split(cmd, ' ');
	path_envp = get_envp_path(environ);
	path_envp += 5;
	splited_path_envp = ft_split(path_envp, ':');
	if (splited_path_envp == NULL)
		exit(1);
	i = 0;
	while (splited_path_envp[i] != NULL)
	{
		path_cmd = join_path(splited_path_envp[i], splited_cmd[0]);
		if (!access(path_cmd, X_OK))
			return (path_cmd);
		free(path_cmd);
		i++;
	}
	return (NULL);
}

int	main(int argc, char *argv[], char **environ)
{
	char	*cmd_path;

	cmd_path = get_path_cmd(argv[argc - 1], environ); // argv[2]
	if (cmd_path == NULL)
		printf("error\n");
	else
		printf("cmd_path = %s\n", cmd_path);
	return (0);
}

__attribute__((destructor))
static void destructor() {
    system("leaks -q a.out");
}
