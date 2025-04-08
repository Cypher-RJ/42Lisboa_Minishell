#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_shell
{
	char	**envp;
	char	*cwd;
	int		exit_status;
}	t_shell;

typedef struct s_redirect
{
	char				*direction;	//! string com direcao
	char				*passorfile;
	struct s_redirect	*redir;
}	t_redirect;

typedef struct s_command
{
	int 				n_cmd;	//! numero de comandos
	char				**args;	//! array para execve
	char				*path;
	t_redirect			*redir;	//! redirecoes seguintes
	struct s_command	*next;
}	t_command;	

void	prompt_loop(char **envp);
char	**split_command(char *input, char **envp);
void	execute_command(char **args, char **envp);
int		detect_redirections(char **args, int *fd_in, int *fd_out);
void	execute_builtin(char **args, char **envp);
int		is_builtin(char *cmd);
void	redirect_output(char *filename, int append);
void	execute_pipe(t_command **cmds, t_shell *shell);
void	redirect_input(char *filename);
char	*get_path(char *cmd, t_shell *shell);
char	*search_path(char *cmd, char **paths);
char	*get_env_path(t_shell *shell);
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
