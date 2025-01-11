/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachhab <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 10:33:40 by olachhab          #+#    #+#             */
/*   Updated: 2025/01/11 15:50:55 by olachhab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int main(int ac, char *av[], char *envp[])
{
	int pipe_fd[2];
	// pipe_fd[0] -> for read ,pipe_fd[1] -> for write
	pid_t pid_1;
	pid_t pid_2;
	int status;

	if (ac != 5)
		error_check("we need: ./pipex infile cmd1 cmd2 outfile\n");
	if (pipe(pipe_fd) == -1)
		error_check("Pipe error");
	pid_1 = fork();
	if (pid_1 < 0)
		error_check("Fork error");

	if (pid_1 == 0)
		first_child(pipe_fd, av, envp);

	pid_2 = fork();
	if (pid_2 < 0)
		error_check("Fork error");

	if (pid_2 == 0)
		second_child(pipe_fd, av, envp);

	waitpid(pid_1, &status, 0);
	waitpid(pid_2, &status, 0);

	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (0);
}
