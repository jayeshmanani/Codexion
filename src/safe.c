/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:38:53 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/18 08:41:47 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*create_safe_malloc(size_t bytes)
{
	void	*mem;

	mem = malloc(bytes);
	if (!mem)
	{
		print_error("Malloc Error: Failed to allocate memory for dongles");
		return (NULL);
	}
	return (mem);
}

int	mutex_safe(t_mtx *mutex, t_pthread_ops ops)
{
	int	status;

	status = 1;
	if (NULL == mutex)
	{
		print_error("Mutex Error: Mutex is NULL");
		return (1);
	}
	if (INIT == ops)
		status = pthread_mutex_init(mutex, NULL);
	else if (LOCK == ops)
		status = pthread_mutex_lock(mutex);
	else if (UNLOCK == ops)
		status = pthread_mutex_unlock(mutex);
	else if (DESTROY == ops)
		status = pthread_mutex_destroy(mutex);
	if (status != 0)
		print_error("Mutex Error: Mutex Operation Failed");
	return (status);
}

int	thread_safe(pthread_t *thread, t_pthread_ops ops, void *(*routine)(void *),
		void *arg)
{
	int	status;

	status = 1;
	if (NULL == thread)
	{
		print_error("Thread Error: Thread is NULL");
		return (1);
	}
	if (CREATE == ops)
		status = pthread_create(thread, NULL, routine, arg);
	else if (JOIN == ops)
		status = pthread_join(*thread, NULL);
	else
		print_error("Wrong Ops for Thread Handling use CREATE or JOIN");
	if (status != 0)
		print_error("Thread Error: Operation Failed");
	return (status);
}
