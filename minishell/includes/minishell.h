/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcesar-d <rcesar-d@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 09:29:09 by rcesar-d          #+#    #+#             */
/*   Updated: 2025/06/23 11:58:30 by rcesar-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "signal_config.h"
# include "../libft/inc/ft_printf.h"
# include "../libft/inc/get_next_line.h"
# include "../libft/inc/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <errno.h>

extern volatile sig_atomic_t	g_signal_status;

typedef struct s_exp
{
	const char	*arg;
	char		**envp;
	int			last_exit;
	char		*result;
	int			i;
	int			in_single;
	int			in_double;
	int			start;
	char		buf[2];
	char		*var;
	char		*val;
}				t_exp;

typedef struct s_redirect
{
	char				*direction;
	char				*passorfile;
	int					hf_fd;
	struct s_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	char				*path;
	t_redirect			*redir;
	struct s_command	*next;
	int					is_first_arg_var;
}						t_command;

typedef struct s_shell
{
	char		**envp;
	int			exit_status;
	t_command	*cmds;
}				t_shell;

int			is_executable(char *filepath);
char		*ft_strjoin_free(char *s1, const char *s2);
char		*ft_strjoin_chr(char *s, char c);
int			print_syntax_error(char *msg);

// env
void		prompt_loop(t_shell *shell);
char		*get_path(char *cmd, t_shell *shell);
char		*search_path(char *cmd, char **paths);
char		*get_env_path(t_shell *shell);
void		increment_shlvl(char **envp);
char		**copy_shlvl(char **envp);
char		*build_prompt(t_shell *shell);
char		*expand_env_variable(const char *arg, char **envp, int last_exit);
void		resolve_path(t_command *head, t_shell *shell);
char		*find_env_value(const char *var, char **envp);

// parse utils
int			check_pipe_and_ampersand(char *input, int *pos);
int			detect_redirections(char **args, int *fd_in, int *fd_out);
int			handle_segment(char *input, char **arg_slot, int *i);
void		handle_input(char *input, t_shell *shell, t_command **cmds);
int			is_only_spaces(char *input);
int			has_unclosed_quotes(char *input);
int			check_syntax(char *input);
int			check_syntax_redir(char *input);
//int			is_single_quoted(char *str);

// build command list
t_command	*build_command_list(char **cmds, t_shell *shell);
t_command	*build_redir(t_command *cmds);
char		*remove_outer_quotes(char *str);
char		**ft_split_quotes(char *str);
char		**split_cmds(char *input);

// pipes & pipe_functions
void		child_pipes(int prev_fd, bool next, int fd[], t_shell *shell);
pid_t		fork_it(void);
int			make_pipe(int fd[], bool has_next);
void		executor_fork(t_shell *shell);

// redirection & heredocs
int			redirector(t_redirect *redir, t_shell *shell, bool has_fork);
int			find_heredocs(t_shell *shell);
void		capture_heredoc(char *line, int fd, t_shell *shell);

//execution
void		executor(t_shell *shell);
void		execute_command(t_command *cmd, t_shell *shell, bool has_fork);
int			execute_builtin(t_command *cmds, t_shell *shell, bool has_fork);
int			check_empty_first(t_shell *shell);

//builtins && builtins_special
int			is_builtin(char *cmd);
int			is_unique_builtin(char *cmd);
void		builtin_pwd(t_shell *shell);
void		builtin_echo(t_command *cmds, t_shell *shell);
void		builtin_env(t_command *cmds, t_shell *shell);
int			builtin_cd(t_command *cmds, t_shell *shell, bool has_fork);
int			builtin_exit(t_command *cmds, t_shell *shell, bool has_fork);
int			builtin_export(t_command *cmds, t_shell *shell, bool has_fork);
int			builtin_unset(t_command *cmds, t_shell *shell, bool has_fork);
int			how_exit(char *msg, bool has_fork, int out, t_shell *shell);

//builtin utils
int			ft_ms_atoll(const char *str, long long *nbr);
int			is_str_numeric(const char *str);
int			builtin_cd_exec(char *strdir, t_shell *shell, bool has_fork);
int			export_putenv(t_shell *shell, bool has_fork);
int			is_var_valid(char *str);
int			export_var(char *str, t_shell *shell);
int			unset_var(char *str, t_shell *shell);
int			rep_add_envp(char *trgt, char *str, t_shell *shell);
char		*build_env_str(char *trgt, char *str);
int			cd_go_home(t_shell *shell, bool has_fork);
int			cd_go_oldpwd(t_command *thiscmd, t_shell *shell, bool has_fork);
char		*get_pwd(t_shell *shell);

//signals
void		setup_signals(void);
void		signal_handler(int sig);
void		pass_signal_status(t_shell	*shell);
void		restore_signals(void);
void		sigint_heredoc_handler(int sig);
void		setup_heredoc_signals(void);
void		wait_for_children(pid_t last_pid, t_shell *shell);
int			signal_fork(int status, int *ctrlc);

//free
void		free_total(t_shell *shell);
void		free_shell(t_shell *shell);
void		free_command_list(t_command *cmd);
void		free_redir_list(t_redirect *redir);
void		free_commands(t_command *head);
void		ft_free_split(char **array);
void		cleanup_and_exit(t_shell *shell, t_command *cmds);

//Extras for norminette compliance
//env_utils.c
void		process_chars(char *str, char *res, int *i, int *j);
void		init_exp(t_exp *exp, const char *arg, char **envp, int last_exit);
void		handle_sord_quotes(int s_or_d, t_exp *exp);
//env_utils_dollar.c
void		handle_dollar(t_exp *exp);
//handle_input_utils.c
void		handle_input_util(int k, t_command **cds, char *inpt, char **strs);
//build_redir.c
void		free_redirect_list(t_redirect *redir_head);
int			process_redirect(t_command *cmds, int i, t_redirect **redir_head, \
				t_redirect **redir_tail);
int			is_redirect_op(char *str);
//parser_utils_quotes.c
int			count_quote_words(char *str);

char		*add_spaces_around_redir(const char *input);
void		check_and_split_expanded_command(t_command *cmd);
void		delete_first_arg(char **args);
void		delete_empty_first_var_arg(t_command *cmd, char **envp, \
				int last_exit);

#endif