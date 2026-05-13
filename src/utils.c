/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 16:38:43 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	error_exit(const char *error)
{
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

void	*malloc_safe_create(size_t bytes)
{
	void	*mem_malloced;

	mem_malloced = malloc(bytes);
	if (NULL == mem_malloced)
		error_exit("Malloc Error: Memory Can not be malloced Properly");
	return (mem_malloced);
}



void	*mutex_safe(t_mtx *mutex, t_mutex_ops ops)
{
	if (INIT == ops)
		pthread_mutex_init(mutex, NULL);
	else if (LOCK == ops)
		pthread_mutex_lock(mutex);
	else if (UNLOCK == ops)
		pthread_mutex_unlock(mutex);
	else if (DESTROY == ops)
		pthread_mutex_destroy(mutex);
	else
		error_exit("Wrong Ops for Mutex Handling");
}
