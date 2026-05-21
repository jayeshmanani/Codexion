/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/21 08:59:10 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile_finish(t_coder *coder)
{
	increase_long(&coder->coder_mutex, &coder->compile_count);
	print_data(COMPILING, coder);
	updated_usleep(coder->cd, coder->cd->compile_time);
	if (coder->cd->n_compiles > 0
		&& coder->compile_count == coder->cd->n_compiles)
		set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
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
	req.deadline_t = get_long(&coder->coder_mutex, &coder->last_compile_t)
		+ coder->cd->burn_time;
	return (req);
}

static int	cancel_from_heap(t_dongle *dongle, int coder_id)
{
	if (mutex_safe(&dongle->dongle_mutex, LOCK) != 0)
		return (1);
	heap_remove(dongle->access_heap, coder_id);
	cond_safe(&dongle->dongle_cond, NULL, BROADCAST, NULL);
	if (mutex_safe(&dongle->dongle_mutex, UNLOCK) != 0)
		return (1);
	return (0);
}

static int	acquire_both_dongles(t_coder *coder)
{
	if (wait_acquire_dongle(coder, coder->left_dongle) != 0)
		return (cancel_from_heap(coder->right_dongle, coder->coder_id), 1);
	print_data(TOOK_DONGLE_1, coder);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		cancel_from_heap(coder->right_dongle, coder->coder_id);
		return (0);
	}
	if (wait_acquire_dongle(coder, coder->right_dongle) != 0)
		return (release_dongle(coder, coder->left_dongle), 1);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		release_dongle(coder, coder->right_dongle);
		return (0);
	}
	print_data(TOOK_DONGLE_2, coder);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	return (0);
}

int	compile(t_coder *coder)
{
	t_req	req;

	if (!coder || !coder->cd)
		return (print_error("Error: Null pointer in compile fn\n"), 1);
	if (coding_finished(coder->cd))
		return (0);
	req = build_req(coder);
	if (pre_register_dongle(coder, coder->left_dongle, req) != 0)
		return (1);
	if (pre_register_dongle(coder, coder->right_dongle, req) != 0)
		return (cancel_from_heap(coder->left_dongle, coder->coder_id), 1);
	if (acquire_both_dongles(coder) != 0)
		return (1);
	if (coding_finished(coder->cd))
		return (0);
	return (compile_finish(coder));
}
