/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:24:32 by jmanani           #+#    #+#             */
/*   Updated: 2026/04/24 17:08:50 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int				g_mails = 0;
pthread_mutex_t	g_mutex;

void	*routine(void)
{
	int	i;

	i = 0;
	while (i < 10000000)
	{
		pthread_mutex_lock(&g_mutex);
		g_mails++;
		pthread_mutex_unlock(&g_mutex);
		i++;
	}
}

// void	*routine2(void)
// {
// 	sleep(2);
// 	printf("R2 Value of x: %d\n", x);
// }

int	main(void)
{
	pthread_t	th[4];
	int			i;
	int			j;
	int			k;

	k = 8;
	i = 0;
	j = 0;
	pthread_mutex_init(&g_mutex, NULL);
	while (i < k)
	{
		if (pthread_create(th + i, NULL, &routine, NULL) != 0)
		{
			printf("Failed to create thread");
			return (1);
		}
		printf("Thread %d is started\n", i);
		i++;
	}
	while (j < k)
	{
		if (pthread_join(th[j], NULL) != 0)
			return (4);
		printf("Thread %d is finished\n", j);
		j++;
	}
	pthread_mutex_destroy(&g_mutex);
	printf("No. of mails: %d\n", g_mails);
	return (0);
}
