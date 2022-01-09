/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lufelipe <lufelipe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 11:21:48 by lufelipe          #+#    #+#             */
/*   Updated: 2022/01/08 11:55:27 by lufelipe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>

typedef struct sigaction	t_sig;

typedef struct args
{
	char		*arg;
	struct args	*next;
}	t_args;

typedef struct cmds
{
	char		*cmd;
	char		*flags;
	t_args		*args;
	struct cmds	*next;
	int			fd_in;
	int			fd_out;
}	t_cmds;

typedef struct variables
{
	int					is_env;
	char				*var;
	char				*value;
	struct variables	*next;
}	t_vars;

typedef struct data
{
	t_cmds				*cmds;
	t_vars				*variables;
	char				**envp;
	t_sig				act;
}	t_data;

extern int					g_reset_fd[3];

t_args	*get_args(char *line, int *count, t_data *data);
void	save_env_var(char *line, int *count, t_data *data, int env);
void	run_aux(t_cmds *cmds, pid_t pid, int *fd, t_sig **act);
void	get_redirect(char *line, int *count, t_cmds *cmds);
void	executor(t_data *data, char ***envp, t_sig **act);
void	if_else(t_cmds *cmds, int fd[]);
void	remove_char(char *s, char c);
void	init_cmds(t_cmds *cmds);
char	*init_run(char **envp, t_cmds *cmds, int fd[], t_sig **act);
char	*treat_input_red(char *line, t_cmds *cmds, t_sig **act);
char	*get_variable(char *line, int size, t_vars *variables);
char	*get_flags(char *line, int *count, t_data *data);
char	*get_cmd(char *line, int *count, t_data *data);
char	*trunc_input_filename(char *line);
char	*change_directory(char *path);
char	get_quote(char *line);
int		run(char **argv, char **envp, t_cmds *cmds, t_sig **act);
int		check_cmds(t_cmds *cmds, char **envp);
int		open_file(char *argv, int i);
int		get_flag_utils(char *line);
int		sintax_check(char *line);
int		file_error(t_cmds *cmds);
int		len_list(t_args *list);
int		ft_isspace(int c);

void	builtin_red(t_cmds *cmds, t_sig **act, int builtin, char **envp);
void	builtin_pwd(t_cmds *cmds);
void	builtin_env(char **envp, t_cmds *cmds);

int		builtin_export(t_cmds *cmds, t_vars **variables, \
char ***envp, t_data *data);
int		builtin_unset(t_cmds *cmds, t_vars **variables, char ***envp);
int		builtin_cd(t_cmds *cmds, t_vars *variables);
int		builtin_exit(t_cmds *cmds, t_data *data);

int		parser(char *line, t_data *data, char ***envp, t_sig **act);

void	exec_builtin(t_cmds *cmds, t_data *data, char ***envp, t_sig **act);
void	cleanup(t_data *data, int end);
void	save_env_var(char *line, int *count, t_data *data, int env);
void	substitute_variables(char **line, t_data *data);
void	lstadd_back(t_data *data, t_vars *new);
void	*ft_calloc(size_t nmemb, size_t size);
void	save_origin_fd(void);
void	reset_input(void);
void	reset_output(void);
void	handle_sigquit(int sig);
void	sigint_handle_cmd(int sig);
void	sigint_handle(int sig);
void	config_sigaction(t_sig *act, void (*handler)(int), int sig);
void	heredoc_sigint(int sig);
int		here_doc(char *limiter, t_sig **act, t_cmds *cmds);

char	*ft_strnstr(const char	*big, const char *little, size_t len);
char	*get_variable(char *line, int size, t_vars *variables);
char	**ft_split(char const *s, char c);
char	*find_path(char *cmd, char **envp);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strndup(const char *s, int len);
char	*cmds_to_string(t_cmds *cmds);
char	*ft_itoa(int n);
char	*get_prompt(void);
char	*ft_strword(const char *s, char *dup);
char	*ft_getinput(char *line, t_cmds *cmds);
char	*ft_concat(char **dest, char *src);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
char	**ft_realloc(char ***array, int size, t_data *data);
t_vars	*get_value(t_vars *variables, char *var);
int		is_builtin(char *cmd);
int		find_env_var(char *var, char **envp);
int		check_unspecified_chars(char *line, t_data *data);
int		execute(t_cmds *cmds, char **envp, t_sig **act, t_data *data);
int		check_quotation(char *line, t_data *data);
int		open_file(char *argv, int i);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		exit_error(int err, char *arg);
int		ft_atoi(const char *str);
int		ft_strlen(char *str);
int		no_file(char *file);
int		sintax_error(void);
int		exec_no_file(t_cmds *cmds);
int		exec_no_perm(t_cmds *cmds);
int		flag_error(t_cmds *cmds);
int		cmd_error(t_cmds *cmds);
void	heredoc_sigquit(int sig);
char	*remove_input_char(char *line);
int		is_builtin(char *cmd);
#endif
