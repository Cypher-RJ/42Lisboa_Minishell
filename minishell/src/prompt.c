#include "../includes/minishell.h"

char	*build_prompt(t_shell *shell)
{
	char	*cwd;
	char	*prompt;
	char	*tmp;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = get_pwd(shell);
	if (!cwd)
		cwd = ft_strdup("unknown");
	tmp = ft_strjoin("\001\033[1;34m\002", cwd);
	free(cwd);
	if (!tmp)
		return (NULL);
	prompt = ft_strjoin_free(tmp, "\001\033[1;35m\002 > \001\033[0m\002");
	return (prompt);
}

void	handle_input(char *input, t_shell *shell, t_command **cmds)
{
	char		**strs;
	t_command	*current;

	if (!*input)
		return ;
	add_history(input);
	strs = split_cmds(input);
	if (!strs)
		return ((void)handle_input_util(0, cmds, input, NULL));
	*cmds = build_command_list(strs, shell);
	current = *cmds;
	while (current)
	{
		if (!build_redir(current))
			return ((void)handle_input_util(1, cmds, input, strs));
		current = current->next;
	}
	ft_free_split(strs);
	resolve_path(*cmds, shell);
	free(input);
}

void	cleanup_and_exit(t_shell *shell, t_command *cmds)
{
	int	exit_code;

	exit_code = shell->exit_status;
	ft_putendl_fd("exit", STDERR_FILENO);
	free_shell(shell);
	if (cmds)
		free_command_list(cmds);
	exit(exit_code);
}

void	no_input_signal(t_shell *shell, char *input)
{
	if (g_signal_status)
		pass_signal_status(shell);
	free(input);
}

void	prompt_loop(t_shell *shell)
{
	char		*input;
	char		*prompt;
	t_command	*cmds;

	cmds = NULL;
	while (1)
	{
		setup_signals();
		prompt = build_prompt(shell);
		input = readline(prompt);
		free(prompt);
		if (!input)
			cleanup_and_exit(shell, cmds);
		if (!*input)
		{
			no_input_signal(shell, input);
			continue ;
		}
		handle_input(input, shell, &cmds);
		pass_signal_status(shell);
		shell->cmds = cmds;
		if (cmds)
			executor(shell);
		cmds = NULL;
	}
}
