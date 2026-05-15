/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/15 18:59:59 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*lone_vibe_coder(void *args)
{
	t_coder	*coder;

	coder = (t_coder *)args;
	waiting_for_coders(coder->cd);
	increase_long(&coder->cd->cd_mutex, &coder->cd->active_coders);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	while (!coding_finished(coder->cd))
		usleep(200);
	return (NULL);
}

void	refactor(t_coder *coder)
{
	coder->refactor_count++;
	print_data(REFACTORING, coder, DEBUG_MODE);
	updated_usleep(coder->cd, coder->cd->refactor_time);
}

void	compile(t_coder *coder)
{
	acquire_dongle(coder->cd, coder->left_dongle);
	print_data(TOOK_DONGLE_1, coder, DEBUG_MODE);
	acquire_dongle(coder->cd, coder->right_dongle);
	print_data(TOOK_DONGLE_2, coder, DEBUG_MODE);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	coder->compile_count++;
	print_data(COMPILING, coder, DEBUG_MODE);
	updated_usleep(coder->cd, coder->cd->compile_time);
	if (coder->cd->n_compiles > 0
		&& coder->compile_count == coder->cd->n_compiles)
		set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
	release_dongle(coder->cd, coder->left_dongle);
	release_dongle(coder->cd, coder->right_dongle);
}
