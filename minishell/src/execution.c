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

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "echo"));
}

void	execute_builtin(char **args, t_shell *shell, bool has_fork)
{//colocar um exit sucess ou error em todos os comandos
	int	i;

	if (!ft_strcmp(args[0], "cd"))
	{
		builtin_cd(args, has_fork);//passar para a funcao
		if (args[1])
		{
			if (chdir(args[1]) != 0)
			{
				perror("cd\n");
				exit (EXIT_FAILURE);
			}
			exit (EXIT_SUCCESS);
		}
		else
		{
			printf("cd: missing argument\n");// se nao tem args, vai para a root.
			exit (EXIT_SUCCESS); 
		}
	}
	else if (!ft_strcmp(args[0], "unset"))
		builtin_unset(args, shell, has_fork);
	else if (!ft_strcmp(args[0], "exit")) // isto precisa de verificar se so tem uma palavra. Acho que se tiver mais passa tudo a string
	{
		builtin_exit(has_fork);
		printf("exit\n");//e suposto escrever algo?
		// mandar limpar tudo!!
		exit(EXIT_SUCCESS);
	}
	else if (!ft_strcmp(args[0], "export"))
		builtin_export(args, shell, has_fork);
	else if (!ft_strcmp(args[0], "pwd"))
	{
		ft_printf("%s\n", getcwd(NULL, 0));
		exit (EXIT_SUCCESS);
	}
	else if (!ft_strcmp(args[0], "echo"))
	{
		i = 1;
		while (args[i])// falta ver se tem -n(nao mete \n no fim)
		{
			write(1 , args[i], ft_strlen(args[i]));
			if (args[i + 1])
				write(1, " ", 1); // acho que o echo transforma todos os argumentos numa unica string
			i++;
		}
		write(1, "\n", 1);
		exit (EXIT_SUCCESS);
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		i = 0;
		while (shell->envp[i])
		{
			printf("%s\n", shell->envp[i]);
			i++;
		}
		exit (EXIT_SUCCESS);
	}
}

int	is_unique_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "exit") || 
		!ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset"));
}

void	executor(t_command *cmds, t_shell *shell)
{
	//Caso seja comando unico e seja cd, exit, export ou unset, nao pode
	//ser feito com fork pois as suas accoes nao ficam capturadas no processo 
	//principal.
	if (cmds->next == NULL && (is_unique_builtin(cmds->args[0]) == 1))
	{
		execute_builtin(cmds->args, shell, 0);
	}
	else
		executor_fork(cmds, shell);
	//Aqui devia limpar tudo ? qd sai tem que passar por aqui, devia limpar aqui tudo.
}

	/*
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
	}*/