/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_cond.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 18:21:36 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 22:13:32 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	cond_error(const char *msg)
{
	print_error(msg);
	return (1);
}

int	cond_safe(pthread_cond_t *cond, t_mtx *mutex, t_pthread_ops ops,
		const struct timespec *ts)
{
	int	status;

	if (NULL == cond)
		return (cond_error("Cond Error: Cond is NULL"));
	if ((WAIT == ops || TIMEDWAIT == ops) && NULL == mutex)
		return (cond_error("Cond Error: Mutex is NULL"));
	status = 1;
	if (INIT == ops)
		status = pthread_cond_init(cond, NULL);
	else if (DESTROY == ops)
		status = pthread_cond_destroy(cond);
	else if (WAIT == ops)
		status = pthread_cond_wait(cond, mutex);
	else if (TIMEDWAIT == ops)
		status = pthread_cond_timedwait(cond, mutex, ts);
	else if (BROADCAST == ops)
		status = pthread_cond_broadcast(cond);
	else
		return (cond_error("Wrong Ops for Cond Handling"));
	if (status != 0 && !(TIMEDWAIT == ops && ETIMEDOUT == status))
		return (cond_error("Cond Error: Cond Operation Failed"));
	return (0);
}
