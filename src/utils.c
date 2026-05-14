/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:06:25 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 17:12:08 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_all(t_coding_data *cd)
{
	int		i;

	i = -1;
	if (NULL == cd)
		err_and_exit("Error - CleanALL: coding_data is NULL in clean_all\n");
	while (++i < cd->n_coders)
	{
		mutex_safe(&cd->coders[i].coder_mutex, DESTROY);
		mutex_safe(&cd->dongles[i].dongle_mutex, DESTROY);
	}
	mutex_safe(&cd->cd_mutex, DESTROY);
	mutex_safe(&cd->op_mutex, DESTROY);
	free(cd->coders);
	free(cd->dongles);
}

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
	if (time_unit == SEC)
		time_in_unit = (time_val.tv_sec) + (time_val.tv_usec / 1e6);
	else if (time_unit == MILLISEC)
		time_in_unit = (time_val.tv_sec * 1e3) + (time_val.tv_usec / 1e3);
	else if (time_unit == MICROSEC)
		time_in_unit = (time_val.tv_sec * 1e6) + (time_val.tv_usec);
	else
		err_and_exit("Error: Invalid time unit");
	return (time_in_unit);
}

void	updated_usleep(t_coding_data *cd, long microsec)
{
	long	start_t;
	long	elapsed_t;
	long	rem_t;

	start_t = get_time(MICROSEC);
	while (get_time(MICROSEC) - start_t < microsec)
	{
		if (coding_finished(cd))
			break ;
		elapsed_t = get_time(MICROSEC) - start_t;
		rem_t = microsec - elapsed_t;
		if (rem_t > 1e3)
			usleep(rem_t / 2);
		else
		{
			while (get_time(MICROSEC) - start_t < microsec)
				;
		}
	}
}
