#include "../includes/minishell.h"

void	execute_command(t_command *cmd, t_shell *shell)
{
	char *errstr;

	errstr = NULL;
	if (cmd->path == NULL)
	{
		errstr = ft_strjoin(cmd->args[0], ": command not found\n");
		perror(errstr);
		free(errstr);
		exit(127);
	}
	if (execve(cmd->path, cmd->args, shell->envp) == -1)
	{
		perror("Failure to execute execve\n");
		exit(1);
	}
}

void	execute_builtin(t_command *cmds, t_shell *shell, bool has_fork)
{
	if (!ft_strcmp(cmds->args[0], "cd"))
		builtin_cd(cmds, has_fork);
	else if (!ft_strcmp(cmds->args[0], "unset"))
		builtin_unset(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "exit"))
		builtin_exit(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "export"))
		builtin_export(cmds, shell, has_fork);
	else if (!ft_strcmp(cmds->args[0], "pwd"))
		builtin_pwd();
	else if (!ft_strcmp(cmds->args[0], "echo"))
		builtin_echo(cmds);
	else if (!ft_strcmp(cmds->args[0], "env"))
		builtin_env(cmds, shell);
}

void	executor(t_command *cmds, t_shell *shell)
{
	//Caso seja comando unico e seja cd, exit, export ou unset, nao pode
	//ser feito com fork pois as suas accoes nao ficam capturadas no processo 
	//principal.
	if (cmds->next == NULL && (is_unique_builtin(cmds->args[0]) == 1))
	{
		execute_builtin(cmds, shell, 0);
	}
	else
		executor_fork(cmds, shell);
	//Aqui devia limpar tudo ? qd sai tem que passar por aqui, devia limpar aqui tudo.
}

/*
void	executor(t_command *cmds, t_shell *shell)
{
	//usar isto para ver se esta a capturar e parsar as linhas 
	//descomentar este e comentar o anterior
	// echo(ou outro) arg1 "arg2" arg3
	// arg3 nao esta a aparecer. argumentos depois de um argumento em "" ja nao estao
	// a entrar 
	int	i;
	int j;

	i = 1;
	while (cmds && shell)
	{
		ft_putnbr_fd(i, 1);
		ft_putendl_fd("o comando:", 1);
		j = 0;
		while (cmds->args[j])
		{
			write(1, "       *", 8);
			write(1, cmds->args[j], ft_strlen(cmds->args[j]));
			ft_putendl_fd("*", 1);
			j++;
		}
		if (cmds->path)
		{
			ft_putendl_fd("    no path:", 1);
			write(1, "       *", 8);
			write(1, cmds->path, ft_strlen(cmds->path));
			ft_putendl_fd("*", 1);
		}
		while (cmds->redir)
		{
			ft_putendl_fd("    com redir:", 1);
			write(1, "       *", 8);
			write(1, cmds->redir->direction, ft_strlen(cmds->redir->direction));
			ft_putendl_fd("*", 1);
			write(1, "       *", 8);
			write(1, cmds->redir->passorfile, ft_strlen(cmds->redir->passorfile));
			ft_putendl_fd("*", 1);
			cmds->redir = cmds->redir->next;
		}
		i++;
		cmds = cmds->next;
	}
}*/