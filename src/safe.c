/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:38:53 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 19:57:56 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	malloc_safe_create(t_coding_data *cd, char c)
{
	if (NULL == cd)
	{
		print_error("Malloc Error: Coding Data is NULL in malloc_safe_create");
		return (1);
	}
	if (c != 'c' && c != 'd')
	{
		print_error("Malloc Error: Wrong type for malloc_safe_create");
		return (1);
	}
	if (c == 'c')
		cd->coders = malloc(cd->n_coders * sizeof(t_coder));
	else if (c == 'd')
		cd->dongles = malloc(cd->n_coders * sizeof(t_dongle));
	if ((c == 'c' && !cd->coders) || (c == 'd' && !cd->dongles))
		return (1);
	return (0);
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
