#include "../includes/minishell.h"

int builtin_cd_exec(char *strdir, t_shell *shell, bool has_fork)
{
    char *old_pwd;
	char *new_pwd;

    old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		return (how_exit("getcwd() failed to allocate", has_fork, 1));
	if (chdir(strdir) != 0)
	{
		free(old_pwd);
		return (how_exit("cd failed", has_fork, 1));
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return (how_exit("getcwd() failed to allocate", has_fork, 1));
	}
    return (0);
}
