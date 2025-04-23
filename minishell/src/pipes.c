#include "../includes/minishell.h"


void	child_labor(int prev_fd, t_command *thiscmd, int fd[], t_shell *shell)
{
	child_pipes(prev_fd, (thiscmd->next != NULL), fd); // se falhar faz exit da child, sem espinhas
	redirector(thiscmd->redir); // se redir != null, faz redir ate == null. Se comeca null, nao faz nada
	execute_builtin(thiscmd->args, shell);//se encontra comando faz exit success ou failure deposi de executar, se nao segue para exec_command
	execute_command(thiscmd, shell);//executa e faz exit success ou exit com erro
}

int	daddy_time(int *prev_fd, bool next, int fd[])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (next == 1)
	{
		close(fd[1]);
		*prev_fd = fd[0];
	}
	return (*prev_fd);
}

void	executor_fork(t_command *cmds, t_shell *shell)
{
	t_command	*thiscmd;
	int			fd[2];
	int			prev_fd;
	pid_t		pid;

	thiscmd = cmds;
	prev_fd = -1;
	while (thiscmd != NULL)
	{
		if (make_pipe(fd, (thiscmd->next != NULL)) == -1)
			break;
		pid = fork_it();
		if (pid == -1)
			break;
		if (pid == 0) // Processo filho
			child_labor(prev_fd, thiscmd, fd, shell);
		prev_fd = daddy_time(&prev_fd, (thiscmd->next != NULL), fd);// So o pai chega aqui
		thiscmd = thiscmd->next;
	}
	thiscmd = cmds;
	while (thiscmd != NULL)
	{
		wait(NULL);
		thiscmd = thiscmd->next;
	}
}

void	executor(t_command *cmds, t_shell *shell)
{
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
	// retirei esta parte pq parece que tenho que fazer sempre um fork
	//para conseguir terminar os comandos de forma limpa
	/*
	if (cmds->next == NULL && is_builtin(cmds->args[0]))
	{
		redirector(cmds->redir, 0);
		execute_builtin(cmds->args, shell, 0);
	}
	else
	{
		executor_fork(cmds, shell);
	}*/
	executor_fork(cmds, shell);
	//Aqui devia limpar tudo ? qd sai tem que passar por aqui, devia limpar aqui tudo.
}
