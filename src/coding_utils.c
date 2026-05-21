/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/21 08:35:38 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	compile_finish(t_coder *coder)
{
	increase_long(&coder->coder_mutex, &coder->compile_count);
	print_data(COMPILING, coder);
	updated_usleep(coder->cd, coder->cd->compile_time);
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
	if (coder->cd->n_compiles > 0
		&& coder->compile_count == coder->cd->n_compiles)
		set_bool(&coder->coder_mutex, &coder->coder_work_done, true);
	if (release_dongle(coder, coder->left_dongle))
		return (1);
	if (release_dongle(coder, coder->right_dongle))
		return (1);
	return (0);
}

static int	compile_helper(t_coder *coder)
{
	if (acquire_dongle(coder, coder->left_dongle))
		return (1);
	print_data(TOOK_DONGLE_1, coder);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		return (0);
	}
	if (acquire_dongle(coder, coder->right_dongle))
	{
		release_dongle(coder, coder->left_dongle);
		return (1);
	}
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		release_dongle(coder, coder->right_dongle);
		return (0);
	}
	print_data(TOOK_DONGLE_2, coder);
	return (compile_finish(coder));
}

int	compile(t_coder *coder)
{
	if (!coder || !coder->cd)
	{
		print_error("Error: Null pointer in compile fn\n");
		return (1);
	}
	if (coding_finished(coder->cd))
		return (0);
	return (compile_helper(coder));
}
