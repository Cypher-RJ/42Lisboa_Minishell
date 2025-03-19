#ifndef MINISHELL_H
# define MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/inc/libft.h"
#include "../libft/inc/get_next_line.h"
#include "../libft/inc/ft_printf.h"
#include <sys/types.h>
#include <sys/wait.h> 

void prompt_loop(char **envp);
char **split_command(char *input);
void execute_command(char **args, char **envp);
void execute_builtin(char **args);
int is_builtin(char *cmd);


#endif