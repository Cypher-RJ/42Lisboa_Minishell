#include "../includes/minishell.h"

char *remove_outer_quotes(char *str)
{
    size_t  len;
    char    *result;
    int     i = 0;
    int     in_single = 0;
    int     in_double = 0;
    
    if (!str)
        return (NULL);
    
    len = ft_strlen(str);
    // Quick check for empty string
    if (len == 0)
        return (str);
    
    // If the string is completely quoted (starts and ends with matching quotes)
    // we need to handle it specially
    if (len >= 2 && 
        ((str[0] == '\'' && str[len - 1] == '\'' && !strchr(str + 1, '\'')) ||
            (str[0] == '"' && str[len - 1] == '"' && !strchr(str + 1, '"'))))
    {
        // This is a completely quoted string - remove the outer quotes only
        result = ft_substr(str, 1, len - 2);
        if (result)
        {
            free(str);
            return (result);
        }
        return (str);
    }
    
    // Otherwise, return the string unchanged since we're preserving inner quotes
    return (str);
}


static char	*find_env_value(char *var_name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(var_name);
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], var_name, len) && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

char *expand_env_variable(const char *arg, char **envp, int last_exit)
{
    int     i = 0;
    int     in_single = 0;
    int     in_double = 0;
    char    *result = ft_strdup("");
    char    *var, *val, *tmp;

    while (arg[i])
    {
        // Handle single quotes - anything inside is literal (including the quotes)
        if (arg[i] == '\'' && !in_double)
        {
            // Toggle single quote state
            in_single = !in_single;
            
            // Add the single quote to the result
            tmp = ft_strjoin_free(result, "'");
            result = tmp;
            i++;
            continue;
        }
        // Handle double quotes - variables get expanded inside
        else if (arg[i] == '"' && !in_single)
        {
            // Toggle double quote state
            in_double = !in_double;
            
            // Add the double quote to the result
            tmp = ft_strjoin_free(result, "\"");
            result = tmp;
            i++;
            continue;
        }
        // Handle environment variables - don't expand inside single quotes
        else if (arg[i] == '$' && !in_single && arg[i + 1])
        {
            // Handle $? special case
            if (arg[i + 1] == '?')
            {
                val = ft_itoa(last_exit);
                tmp = ft_strjoin_free(result, val);
                free(val);
                result = tmp;
                i += 2; // Skip "$?"
                continue;
            }
            // Handle normal environment variables
            int start = ++i;
            while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
                i++;
            
            // Extract variable name and get its value
            var = ft_substr(arg, start, i - start);
            val = find_env_value(var, envp);
            free(var);
            
            // Add the value to our result
            tmp = ft_strjoin_free(result, val);
            free(val);
            result = tmp;
        }
        else
        {
            // Add any other character directly
            char buf[2] = { arg[i], '\0' };
            tmp = ft_strjoin_free(result, buf);
            result = tmp;
            i++;
        }
    }
    return (result);
}

static int	handle_redirection(char **args, int *fd, int flags, int i)
{
	if (*fd != -1)
		close(*fd);
	*fd = open(args[i + 1], flags, 0644);
	if (*fd < 0)
	{
		perror("open");
		return (1);
	}
	args[i] = NULL;
	args[i + 1] = NULL;
	return (0);
}

int	detect_redirections(char **args, int *fd_in, int *fd_out)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0)
		{
			if (!args[i + 1])
				return (fprintf(stderr, "minishell: syntax error near `>`\n"),
					1);
			if (handle_redirection(args, fd_out, O_WRONLY | O_CREAT | O_TRUNC,
					i))
				return (1);
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			if (!args[i + 1])
				return (fprintf(stderr, "minishell: syntax error near `<`\n"),
					1);
			if (handle_redirection(args, fd_in, O_RDONLY, i))
				return (1);
		}
		i++;
	}
	return (0);
}
