#include "../includes/minishell.h"

void	manage_execve(int err, char *path, t_shell *shell, bool has_fork)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	if (err == EACCES)
		how_exit(": Permission denied", has_fork, 126, shell);
	else if (err == ENOENT)
		how_exit(": No such file or directory", has_fork, 127, shell);
	else if (errno == EISDIR)
		how_exit(": Is a directory", has_fork, 126, shell);
	else if (errno == ENOEXEC)
		how_exit(": Exec format error", has_fork, 126, shell);
	else
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		how_exit(strerror(err), has_fork, 1, shell);
	}
}

void	execute_command(t_command *cmd, t_shell *shell, bool has_fork)
{
	struct stat	st;

	if (cmd->args[0] == NULL || cmd->args[0][0] == 0)
		how_exit(NULL, has_fork, EXIT_SUCCESS, shell);
	if (cmd->path == NULL)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		how_exit(": command not found", has_fork, 127, shell);
	}
	if (stat(cmd->path, &st) == 0 && (st.st_mode & S_IFMT) == S_IFDIR)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->path, STDERR_FILENO);
		how_exit(": Is a directory", has_fork, 126, shell);
	}
	if (execve(cmd->path, cmd->args, shell->envp) == -1)
		manage_execve(errno, cmd->path, shell, has_fork);
}

int	execute_builtin(t_command *cmds, t_shell *shell, bool has_fork)
{
	int	out;

	out = 0;
	if (!has_fork && cmds->redir)
	{
		if (redirector(cmds->redir, shell, 0) != 0)
			return (how_exit(NULL, 0, EXIT_FAILURE, shell));
	}
	if (!ft_strcmp(cmds->args[0], "cd"))
		out = builtin_cd(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "unset"))
		out = builtin_unset(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "exit"))
		out = builtin_exit(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "export"))
		out = builtin_export(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "pwd"))
		builtin_pwd(shell);
	else if (!ft_strcmp(cmds->args[0], "echo"))
		builtin_echo(cmds, shell);
	else if (!ft_strcmp(cmds->args[0], "env"))
		builtin_env(cmds, shell);
	return (out);
}

void	executor(t_shell *shell)
{
	if (find_heredocs(shell))
		return ;
	if (shell->cmds->next == NULL && shell->cmds->args[0] &&
		(is_unique_builtin(shell->cmds->args[0]) == 1))
		execute_builtin(shell->cmds, shell, 0);
	else
		executor_fork(shell);
	free_command_list(shell->cmds);
	shell->cmds = NULL;
}

/*
void	executor(t_shell *shell)
{
	//usar isto para ver se esta a capturar e parsar as linhas 
	//descomentar este e comentar o anterior
	// echo(ou outro) arg1 "arg2" arg3
	// arg3 nao esta a aparecer. argumentos depois de um argumento em "" ja nao estao
	// a entrar 
	int	i;
	int j;

	i = 1;
	while (shell && shell->cmds)
	{
		ft_putnbr_fd(i, 1);
		ft_putendl_fd("o comando:", 1);
		j = 0;
		while (shell->cmds->args[j])
		{
			write(1, "       *", 8);
			write(1, shell->cmds->args[j], ft_strlen(shell->cmds->args[j]));
			ft_putendl_fd("*", 1);
			j++;
		}
		if (shell->cmds->path)
		{
			ft_putendl_fd("    no path:", 1);
			write(1, "       *", 8);
			write(1, shell->cmds->path, ft_strlen(shell->cmds->path));
			ft_putendl_fd("*", 1);
		}
		while (shell->cmds->redir)
		{
			ft_putendl_fd("    com redir:", 1);
			write(1, "       *", 8);
			write(1, shell->cmds->redir->direction, ft_strlen(shell->cmds->redir->direction));
			ft_putendl_fd("*", 1);
			write(1, "       *", 8);
			write(1, shell->cmds->redir->passorfile, ft_strlen(shell->cmds->redir->passorfile));
			ft_putendl_fd("*", 1);
			shell->cmds->redir = shell->cmds->redir->next;
		}
		i++;
		shell->cmds = shell->cmds->next;
	}
}
*/
//  valgrind --suppressions=readline.supp --show-leak-kinds=all --leak-check=full --track-origins=yes --track-fds=yes ./minishell
