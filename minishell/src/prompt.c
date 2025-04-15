#include "../includes/minishell.h"

char	*build_prompt(void)
{
	char	*cwd;
	char	*prompt;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = ft_strdup("unknown");
	prompt = ft_strjoin("\001\033[1;34m\002", cwd); // Add BOLD_BLUE for cwd
	free(cwd);
	prompt = ft_strjoin_free(prompt, "\001\033[1;35m\002 > \001\033[0m\002");
	return (prompt);
}

/* void	handle_input(char *input, t_shell *shell)
{
	char	***args;

	add_history(input);
	args = split_command(input, shell->envp);
	if (args[0])
	{
		if (is_builtin(args[0]))
			execute_builtin(args, shell->envp);
		else
			execute_command(args, shell->envp);
	}
	ft_free_split(args);
} */

void	prompt_loop(t_shell *shell)
{
	char *input;
	char *prompt;
	t_command *cmds;
	char **strs;

	while (1)
	{
		setup_signals();
		prompt = build_prompt();
		input = readline(prompt);
		add_history(input);
		free(prompt);
		strs = split_cmds(input);
		cmds = build_command_list(strs, shell);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		// handle_input(input, shell->envp);
		free(input);
	}
}