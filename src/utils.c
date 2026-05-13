/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 00:11:46 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	err_and_exit(const char *error)
{
	if (NULL == error)
		error = "ErrorEnd: An unknown error occurred.\n";
	printf("%s\n", error);
	exit(EXIT_FAILURE);
}

long	get_time(t_time_unit time_unit)
{
	struct timeval	time_val;
	long			time_in_unit;

	if (gettimeofday(&time_val, NULL))
		err_and_exit("Error: gettimeofday failed");
	if (time_unit == S)
		time_in_unit = (time_val.tv_sec) + (time_val.tv_usec / 1e6);
	else if (time_unit == milliS)
		time_in_unit = (time_val.tv_sec * 1e3) + (time_val.tv_usec / 1e3);
	else if (time_unit == microS)
		time_in_unit = (time_val.tv_sec * 1e6) + (time_val.tv_usec);
	else
		err_and_exit("Error: Invalid time unit");
	return (time_in_unit);
}
