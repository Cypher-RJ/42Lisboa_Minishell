#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>

typedef struct s_shell
{
	char	**envp;
	// char	*cwd;
	int		exit_status;
}			t_shell;

typedef struct s_redirect
{
	char				*direction; //! string com direcao
	char				*passorfile;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args; //! array para execve ou builtin
	char				*path; //path para execve
	t_redirect			*redir; //! redirecoes seguintes - pra builtins fica null
	struct s_command	*next;
}						t_command;

void		prompt_loop(t_shell *shell);
int			detect_redirections(char **args, int *fd_in, int *fd_out);
char		*get_path(char *cmd, t_shell *shell);
char		*search_path(char *cmd, char **paths);
char		*get_env_path(t_shell *shell);
int			is_executable(char *filepath);
void		setup_signals(void);
void		signal_handler(int sig);
char		*expand_env_variable(char *arg, char **envp);
void		ft_free_split(char **array);
void		increment_shlvl(char **envp);
char		**copy_shlvl(char **envp);
char		*ft_strjoin_free(char *s1, const char *s2);
void		handle_input(char *input, char **envp);
char		*build_prompt(void);
char		*ft_strjoin_chr(char *s, char c);
char		**split_cmds(char *input);
int			handle_segment(char *input, char **arg_slot, int *i);
int			check_pipe_and_ampersand(char *input, int *i, int *expect);
int			is_only_spaces(char *input);
int			has_unclosed_quotes(char *input);
int			check_syntax(char *input);
int			print_syntax_error(char *msg);
t_command	*build_command_list(char **cmds, t_shell *shell);
void		free_commands(t_command *head);
int			check_syntax_redir(char *input);
char		**ft_split_quotes(char *str);
int			is_single_quoted(char *str);

// pipes & pipe_functions
void		executor(t_command **cmds, t_shell *shell);
void		child_pipes(int prev_fd, bool next, int fd[]);
pid_t		fork_it();
int			make_pipe(int fd[], bool has_next);

// redirection
void		redirector(t_redirect *redir);

//execution
void		execute_command(t_command *cmd, t_shell *shell);
void		execute_builtin(char **args, t_shell *shell);
int			is_builtin(char *cmd);

#endif
