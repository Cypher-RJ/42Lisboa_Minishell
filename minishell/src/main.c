#include "minishell.h"

void prompt_loop(char **envp)
{
	char *input;
	char **args;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		add_history(input);

		args = split_command(input);
		if(args[0])
		{
			if (is_builtin(args[0]))
				execute_builtin(args);
			else
				execute_command(args, envp);
		}

		free(input);
		free(args);
	}
}

int main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	
	prompt_loop(envp);
	return(0);
}