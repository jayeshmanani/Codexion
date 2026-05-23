/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 14:21:08 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile_finish(t_coder *coder)
{
	increase_long(&coder->coder_mutex, &coder->compile_count);
	print_data(COMPILING, coder);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	updated_usleep(coder->cd, coder->cd->compile_time);
	if (coder->cd->n_compiles > 0
		&& coder->compile_count == coder->cd->n_compiles)
		mark_coder_done(coder);
	if (release_dongle(coder, coder->left_dongle))
		return (1);
	if (release_dongle(coder, coder->right_dongle))
		return (1);
	return (0);
}

static t_req	build_req(t_coder *coder)
{
	t_req	req;

	req = coder->coder_req;
	req.arrival_t = get_time(MILLISEC);
	req.deadline_t = get_long(&coder->coder_mutex, &coder->last_compile_t)
		+ coder->cd->burn_time;
	coder->coder_req.arrival_t = req.arrival_t;
	coder->coder_req.deadline_t = req.deadline_t;
	return (req);
}

int	compile(t_coder *coder)
{
	t_req	req;

	if (!coder || !coder->cd)
		return (print_error("Error: Null pointer in compile fn\n"), 1);
	if (coding_finished(coder->cd))
		return (0);
	if (coder->cd->n_coders == 1)
	{
		print_data(TOOK_DONGLE_1, coder);
		while (!coding_finished(coder->cd))
			usleep(500);
		return (0);
	}
	req = build_req(coder);
	if (register_global_request(coder, req) != 0)
		return (1);
	if (wait_acquire_both_dongles(coder) != 0)
		return (1);
	print_data(TOOK_DONGLE_1, coder);
	print_data(TOOK_DONGLE_2, coder);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		release_dongle(coder, coder->right_dongle);
		return (0);
	}
	return (compile_finish(coder));
}
