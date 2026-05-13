/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 16:38:53 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 17:00:04 by jmanani          ###   ########.fr       */
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

static void	handle_mutex_return(int status, t_mutex_ops ops)
{
	if (0 == status)
		return ;
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

void	mutex_safe(t_mtx *mutex, t_mutex_ops ops)
{
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
