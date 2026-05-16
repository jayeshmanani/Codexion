/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:38:53 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 18:24:11 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*malloc_safe_create(size_t bytes)
{
	void	*mem_malloced;

	mem_malloced = malloc(bytes);
	if (NULL == mem_malloced)
		err_and_exit("Malloc Error: Memory Can not be malloced Properly");
	return (mem_malloced);
}

static void	handle_mutex_return(int status, t_pthread_ops ops)
{
	if (0 == status)
		return ;
	// if (ops)
	// 	err_and_exit("Mutex Error: Mutex Operation Failed");
	if (EINVAL == status && (INIT == ops || UNLOCK == ops))
		err_and_exit("Mutex Attribute is Invalid");
	else if (EINVAL == status && (LOCK == ops || UNLOCK == ops))
		err_and_exit("Mutex value is Invalid");
	else if (EDEADLK == status)
		err_and_exit("Deadlock can occur: Dongle blocked waiting for mutex");
	else if (EPERM == status)
		err_and_exit("Dongle is not holding the lock on Mutex");
	else if (ENOMEM == status)
		err_and_exit("Process can't allocate enough memory for another mutex");
	else if (EBUSY == status)
		err_and_exit("Mutex is already locked");
}

void	mutex_safe(t_mtx *mutex, t_pthread_ops ops)
{
	if (NULL == mutex)
		err_and_exit("Mutex Error: Mutex is NULL");
	if (INIT == ops)
		handle_mutex_return(pthread_mutex_init(mutex, NULL), ops);
	else if (LOCK == ops)
		handle_mutex_return(pthread_mutex_lock(mutex), ops);
	else if (UNLOCK == ops)
		handle_mutex_return(pthread_mutex_unlock(mutex), ops);
	else if (DESTROY == ops)
		handle_mutex_return(pthread_mutex_destroy(mutex), ops);
	else
		err_and_exit("Wrong Ops for Mutex Handling");
}

static void	handle_thread_return(int status, t_pthread_ops ops)
{
	if (0 == status)
		return ;
	// if (ops)
	// 	err_and_exit("Thread Error: Thread Operation Failed");
	if (EAGAIN == status)
		err_and_exit("Resource limits for threads exceeded");
	else if (EPERM == status)
		err_and_exit("No permission to set attribute for thread");
	else if (EINVAL == status && CREATE == ops)
		err_and_exit("Invalid value as attribute");
	else if (EINVAL == status && (JOIN == ops))
		err_and_exit("Thread is not joinable");
	else if (ESRCH == status)
		err_and_exit("No thread could be found with the ID specified");
	else if (EDEADLK == status)
		err_and_exit("A deadlock was detected or the thread is already joined");
}

void	thread_safe(pthread_t *thread, t_pthread_ops ops,
		void *(*routine)(void *), void *arg)
{
	if (NULL == thread)
		err_and_exit("Thread Error: Thread is NULL");
	if (CREATE == ops)
		handle_thread_return(pthread_create(thread, NULL, routine, arg), ops);
	else if (JOIN == ops)
		handle_thread_return(pthread_join(*thread, NULL), ops);
	else
		err_and_exit("Wrong Ops for Thread Handling use CREATE or JOIN");
}
