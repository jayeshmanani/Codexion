/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_queue_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 11:02:12 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:53:14 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	lock_dongles_pair(t_dongle *a, t_dongle *b)
{
	if (a->dongle_id > b->dongle_id)
	{
		if (mutex_safe(&b->dongle_mutex, LOCK) != 0)
			return (1);
		if (mutex_safe(&a->dongle_mutex, LOCK) != 0)
		{
			mutex_safe(&b->dongle_mutex, UNLOCK);
			return (1);
		}
		return (0);
	}
	if (mutex_safe(&a->dongle_mutex, LOCK) != 0)
		return (1);
	if (mutex_safe(&b->dongle_mutex, LOCK) != 0)
	{
		mutex_safe(&a->dongle_mutex, UNLOCK);
		return (1);
	}
	return (0);
}

void	unlock_dongles_pair(t_dongle *a, t_dongle *b)
{
	if (a->dongle_id > b->dongle_id)
	{
		mutex_safe(&a->dongle_mutex, UNLOCK);
		mutex_safe(&b->dongle_mutex, UNLOCK);
		return ;
	}
	mutex_safe(&b->dongle_mutex, UNLOCK);
	mutex_safe(&a->dongle_mutex, UNLOCK);
}

void	reset_dongles_pair(t_dongle *a, t_dongle *b)
{
	if (lock_dongles_pair(a, b) != 0)
		return ;
	a->is_taken = false;
	b->is_taken = false;
	unlock_dongles_pair(a, b);
}

int	take_both_if_ready(t_coder *coder, long *wait_msec)
{
	long	now;
	long	wait_left;
	long	wait_right;

	if (lock_dongles_pair(coder->left_dongle, coder->right_dongle) != 0)
		return (1);
	if (coder->left_dongle->is_taken || coder->right_dongle->is_taken)
		*wait_msec = -1;
	else
	{
		now = get_time(MILLISEC);
		wait_left = coder->left_dongle->next_available_t - now;
		wait_right = coder->right_dongle->next_available_t - now;
		*wait_msec = wait_left;
		if (wait_right > *wait_msec)
			*wait_msec = wait_right;
		if (*wait_msec <= 0)
		{
			coder->left_dongle->is_taken = true;
			coder->right_dongle->is_taken = true;
			*wait_msec = 0;
		}
	}
	unlock_dongles_pair(coder->left_dongle, coder->right_dongle);
	return (0);
}
