/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:24:32 by jmanani           #+#    #+#             */
/*   Updated: 2026/04/24 16:57:37 by jmanani          ###   ########.fr       */
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
	while (i < 1000000)
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
	pthread_t	t1;
	pthread_t	t2;

	pthread_mutex_init(&g_mutex, NULL);
	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
		return (2);
	if (pthread_join(t1, NULL) != 0)
		return (3);
	if (pthread_join(t2, NULL) != 0)
		return (4);
	pthread_mutex_destroy(&g_mutex);
	printf("No. of mails: %d\n", g_mails);
	return (0);
}
