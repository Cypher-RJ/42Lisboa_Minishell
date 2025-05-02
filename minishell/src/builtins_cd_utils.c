#include "../includes/minishell.h"

int builtin_cd_exec(char *strdir, t_shell *shell, bool has_fork)
{
    char *old_pwd;
	char *new_pwd;

    old_pwd = getcwd(NULL, 0);
	if (!old_pwd && shell)
		return (how_exit("getcwd() failed to allocate", has_fork, \
			EXIT_FAILURE, shell));
	if (chdir(strdir) != 0)
	{
		free(old_pwd);
		return (how_exit("cd failed", has_fork, EXIT_FAILURE, shell));
	}
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		free(old_pwd);
		return (how_exit("getcwd() failed to allocate", has_fork, \
			EXIT_FAILURE, shell));
	}
    return (how_exit(NULL, has_fork, EXIT_SUCCESS, shell));
}
