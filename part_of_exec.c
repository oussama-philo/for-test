/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_of_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachhab <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:50:18 by olachhab          #+#    #+#             */
/*   Updated: 2025/01/12 14:38:12 by olachhab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void path_execution(char **args, char *envp[])
{
	char **paths;
	char *cmd_path;
	int i;

	paths = find_path(envp);
	if (!paths)
		return;
	i = 0;
	while (paths[i])
	{
		cmd_path = join_paths(paths[i], args[0]);
		if (cmd_path)
		{
			if (access(cmd_path, X_OK) == 0)
			{
				execve(cmd_path, args, envp);
				free(cmd_path);
				break;
			}
			free(cmd_path);
		}
		i++;
	}
	free_array(paths);
	error_check("Command not found path_execution");
}

void execute_cmd(char *argv[], char *envp[], int is_first)
{
	char **args;
	char *cmd;

	if (is_first == 1)
		args = split_cmd(argv[2], " '");
	else
		args = split_cmd(argv[3], " '");
	if(!args || !args[0])
		error_check("Invalid command");
	cmd = args[0];
	if (cmd[0] == '/')
	{
		if (access(cmd, X_OK) == 0)
			execve(cmd, args, envp);
	}
	else
		path_execution(args, envp);
	free_array(args);
	error_check("Command not found execute_cmd");
}

void second_child(int *pipe_fd, char **av, char **envp)
{
	int outfile;

		outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile < 0)
			error_check("Outfile File not exist");

	close(pipe_fd[1]);
	dup2(outfile, STDOUT_FILENO);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(outfile);
	close(pipe_fd[0]);
	execute_cmd(av, envp, 0);
}

void first_child(int *pipe_fd, char **av, char **envp)
{
	int infile;

	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		error_check("Infile error");
	close(pipe_fd[0]);
	dup2(infile, STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(infile);
	close(pipe_fd[1]);
	execute_cmd(av, envp, 1);
}
