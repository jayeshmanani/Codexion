/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:09:28 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 18:52:21 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_burned_out(t_coder *coder)
{
	long	elapsed_time;
	long	last_compile_time;

	if (NULL == coder)
		err_and_exit("Error: coder is NULL in coder_burned_out\n");
	if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
		return (false);
	last_compile_time = get_long(&coder->coder_mutex, &coder->last_compile_t);
	if (last_compile_time == 0)
		return (false);
	elapsed_time = get_time(MILLISEC) - last_compile_time;
	return (elapsed_time > coder->cd->burn_time);
}

static void	shutdown_all(t_coding_data *cd)
{
	int	j;

	set_bool(&cd->cd_mutex, &cd->end_coding, true);
	j = -1;
	while (++j < cd->n_coders)
	{
		if (cond_safe(&cd->coders[j].coder_req_cond, NULL, BROADCAST,
				NULL) != 0)
			err_and_exit("Error: cond_safe failed in shutdown_all\n");
		if (cond_safe(&cd->dongles[j].dongle_cond, NULL, BROADCAST, NULL) != 0)
			err_and_exit("Error: cond_safe failed in shutdown_all\n");
	}
}

void	*coding_analyser(void *args)
{
	t_coding_data	*cd;
	int				i;

	cd = (t_coding_data *)args;
	while (!all_coders_ready(&cd->cd_mutex, &cd->active_coders, cd->n_coders))
		usleep(500);
	while (!coding_finished(cd))
	{
		i = -1;
		while (++i < cd->n_coders && !coding_finished(cd))
		{
			if (coder_burned_out(cd->coders + i))
			{
				shutdown_all(cd);
				print_data(BURNED_OUT, cd->coders + i, DEBUG_MODE);
			}
		}
		usleep(100);
	}
	return (NULL);
}
