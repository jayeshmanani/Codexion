/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 14:54:11 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/18 08:11:29 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time(t_time_unit time_unit)
{
	struct timeval	time_val;
	long			time_in_unit;

	time_in_unit = 0;
	if (gettimeofday(&time_val, NULL) != 0)
	{
		print_error("Error: gettimeofday failed");
		return (0);
	}
	if (time_unit == SEC)
		time_in_unit = (time_val.tv_sec) + (time_val.tv_usec / 1e6);
	else if (time_unit == MILLISEC)
		time_in_unit = (time_val.tv_sec * 1e3) + (time_val.tv_usec / 1e3);
	else if (time_unit == MICROSEC)
		time_in_unit = (time_val.tv_sec * 1e6) + (time_val.tv_usec);
	else
		print_error("Error: Invalid time unit");
	return (time_in_unit);
}

void	updated_usleep(t_coding_data *cd, long millisec)
{
	long	start_t;
	long	elapsed_t;
	long	rem_t;

	start_t = get_time(MICROSEC);
	while (get_time(MICROSEC) - start_t < millisec * 1e3)
	{
		if (coding_finished(cd))
			break ;
		elapsed_t = get_time(MICROSEC) - start_t;
		rem_t = (millisec * 1e3) - elapsed_t;
		if (rem_t > 1e3)
			usleep(rem_t / 2);
		else
		{
			while (get_time(MICROSEC) - start_t < (millisec * 1e3))
				;
		}
	}
}

void	abs_time_from_usec(long abs_usec, struct timespec *ts)
{
	ts->tv_sec = abs_usec / 1000000;
	ts->tv_nsec = (abs_usec % 1000000) * 1000;
}
