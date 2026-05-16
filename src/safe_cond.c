/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_cond.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/16 18:21:36 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 18:22:22 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cond_safe(pthread_cond_t *cond, t_mtx *mutex,
            t_pthread_ops ops, const struct timespec *ts)
{
    int	status;

    if (NULL == cond)
        err_and_exit("Cond Error: Cond is NULL");
    if ((WAIT == ops || TIMEDWAIT == ops) && NULL == mutex)
        err_and_exit("Cond Error: Mutex is NULL");

    if (WAIT == ops)
        status = pthread_cond_wait(cond, mutex);
    else if (TIMEDWAIT == ops)
        status = pthread_cond_timedwait(cond, mutex, ts);
    else if (BROADCAST == ops)
        status = pthread_cond_broadcast(cond);
    else
        err_and_exit("Wrong Ops for Cond Handling");

    if (0 == status)
        return ;
    if (TIMEDWAIT == ops && ETIMEDOUT == status)
        return ;
    err_and_exit("Cond Error: Cond Operation Failed");
}
