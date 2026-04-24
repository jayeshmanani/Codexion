/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 14:24:32 by jmanani           #+#    #+#             */
/*   Updated: 2026/04/24 16:37:44 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int		mails = 0;

void	*routine(void)
{
	int	i;

	i = 0;
	while (i < 1000000)
	{
		mails++;
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

	if (pthread_create(&t1, NULL, &routine, NULL) != 0)
		return (1);
	if (pthread_create(&t2, NULL, &routine, NULL) != 0)
		return (2);
	if (pthread_join(t1, NULL) != 0)
		return (3);
	if (pthread_join(t2, NULL) != 0)
		return (4);
	printf("No. of mails: %d\n", mails);
	return (0);
}
