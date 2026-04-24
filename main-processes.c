/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main-processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:46:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/04/24 16:23:06 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid1;
	int		x;

	x = 2;
	pid1 = fork();
	if (pid1 == 0)
	{
		x++;
		printf("Child 1 PID: %d\n", getpid());
	}
	printf("value of x: %d\n", x);
	waitpid(pid1, NULL, 0);
	printf("PID: %d\n", getpid());
	return (0);
}
