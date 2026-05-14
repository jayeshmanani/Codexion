/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 15:09:28 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 16:31:42 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_burned_out(t_coder *coder)
{
	long	elapsed_time;
	long	t_to_die;

	if (NULL == coder)
		err_and_exit("Error: coder is NULL in coder_burned_out\n");
	if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
		return (false);
	elapsed_time = get_time(MILLISEC) - get_long(&coder->coder_mutex,
			&coder->last_compile_t);
	t_to_die = coder->cd->burn_time / 1e3;
	return (elapsed_time > t_to_die);
}

void	*coding_analyser(void *args)
{
	t_coding_data	*cd;
	int				i;

	cd = (t_coding_data *)args;
	while (!all_coders_ready(&cd->cd_mutex, &cd->active_coders,
			cd->n_coders))
		;
	while (!coding_finished(cd))
	{
		i = -1;
		while (++i < cd->n_coders && !coding_finished(cd))
		{
			// printf("coder %d\n", i + 1);
            // printf("Coder: %p\n", (void *)(cd->coders + i)); // Debug statement
			if (coder_burned_out(cd->coders + i))
			{
				set_bool(&cd->cd_mutex, &cd->end_coding, true);
				print_data(BURNED_OUT, cd->coders + i, DEBUG_MODE);
			}
		}
	}
	return (NULL);
}
