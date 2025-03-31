#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>

void	prompt_loop(char **envp);
char	**split_command(char *input, char **envp);
void	execute_command(char **args, char **envp);
int		detect_redirections(char **args, int *fd_in, int *fd_out);
void	execute_builtin(char **args, char **envp);
int		is_builtin(char *cmd);
void	redirect_output(char *filename, int append);
void	execute_pipe(char **cmd1, char **cmd2, char **envp);
void	redirect_input(char *filename);
char	*get_path(char *cmd, char **envp);
char	*search_path(char *cmd, char **paths);
char	*get_env_path(char **envp);
int		is_executable(char *filepath);
void	setup_signals(void);
void	signal_handler(int sig);
char	*expand_env_variable(char *arg, char **envp);
void	ft_free_split(char **array);
void	increment_shlvl(char **envp);
char	**copy_shlvl(char **envp);
char	*ft_strjoin_free(char *s1, const char *s2);
void	handle_input(char *input, char **envp);
char	*build_prompt(void);

#endif
