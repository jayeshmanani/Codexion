/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:09:28 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 19:33:48 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_burned_out(t_coder *coder)
{
	long	elapsed_time;
	long	last_compile_time;

	if (NULL == coder)
	{
		print_error("Error: coder is NULL in coder_burned_out\n");
		return (false);
	}
	if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
		return (false);
	last_compile_time = get_long(&coder->coder_mutex, &coder->last_compile_t);
	if (last_compile_time == 0)
		return (false);
	elapsed_time = get_time(MILLISEC) - last_compile_time;
	return (elapsed_time > coder->cd->burn_time);
}

void	*coding_analyser(void *args)
{
	t_coding_data	*cd;
	int				i;

	cd = (t_coding_data *)args;
	if (NULL == cd)
		return (NULL);
	while (!all_coders_ready(&cd->cd_mutex, &cd->active_coders, cd->n_coders))
		usleep(500);
	while (!coding_finished(cd))
	{
		i = -1;
		while (++i < cd->n_coders && !coding_finished(cd))
		{
			if (coder_burned_out(cd->coders + i))
			{
				signal_end(cd);
				print_data(BURNED_OUT, cd->coders + i);
			}
		}
		usleep(500);
	}
	return (NULL);
}
