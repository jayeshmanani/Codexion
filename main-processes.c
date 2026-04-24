/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main-processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:46:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/04/24 14:53:43 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 == 0)
	{
		printf("Child 1 PID: %d\n", getpid());
		return (0);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		printf("Child 2 PID: %d\n", getpid());
		return (0);
	}
	// Only parent reaches here
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	printf("Parent PID: %d\n", getpid());
	return (0);
}
