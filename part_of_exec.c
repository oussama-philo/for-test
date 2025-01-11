/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_of_exec.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachhab <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:50:18 by olachhab          #+#    #+#             */
/*   Updated: 2025/01/11 15:52:16 by olachhab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	path_execution(char **args, char *envp[])
{
	char	**paths;
	char	*cmd_path;
	int		i;

	paths = find_path(envp);
	i = 0;
	while (paths[i])
	{
		cmd_path = join_paths(paths[i], args[0]);
		if (access(cmd_path, X_OK) == 0)
		{
			execve(cmd_path, args, envp);
			free(cmd_path);
		}
		free(cmd_path);
		i++;
	}
	free_array(paths);
	free_array(args);
	error_check("Command not found path_execution");
}

void	execute_cmd(char *argv[], char *envp[], int is_first)
{
	char	**args;

	if (is_first == 1)
		args = split_cmd(argv[2], " '");
	else
		args = split_cmd(argv[3], " '");
	if (args[0][0] == '/')
	{
		if (access(args[0], X_OK) == 0)
			execve(args[0], args, envp);
		free_array(args);
		error_check("Command not found execute_cmd");
	}
	path_execution(args, envp);
}

void	second_child(int *pipe_fd, char **av, char **envp)
{
	int		outfile;
	size_t	byte_read;
	char	buffer[1024];

	if (access("outfile", F_OK) == 0)
	{
		outfile = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (outfile < 0)
			error_check("Outfile File not exist");
	}
	close(pipe_fd[1]);
	while ((byte_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0)
	{
		if (write(outfile, buffer, byte_read) != byte_read)
			error_check("Write error");
	}
	if (byte_read < 0)
		error_check("Read error");
	close(outfile);
	close(pipe_fd[0]);
	execute_cmd(av, envp, 0);
}

void	first_child(int *pipe_fd, char **av, char **envp)
{
	int		infile;
	size_t	byte_read;
	char	buffer[1024];

	infile = open(av[1], O_RDONLY);
	if (infile < 0)
		error_check("Infile error");
	close(pipe_fd[0]);
	while ((byte_read = read(infile, buffer, sizeof(buffer))) > 0)
	{
		if (write(pipe_fd[1], buffer, byte_read) != byte_read)
			error_check("Write error");
	}
	if (byte_read < 0)
		error_check("Read error");
	close(infile);
	close(pipe_fd[1]);
	execute_cmd(av, envp, 1);
}
