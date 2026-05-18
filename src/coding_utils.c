/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 18:58:22 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/18 06:46:06 by jmanani          ###   ########.fr       */
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
	while (!coding_finished(coder->cd))
	{
		if (get_bool(&coder->coder_mutex, &coder->coder_work_done))
			break ;
		if (compile(coder) != 0)
			return (NULL);
		if (!coding_finished(coder->cd))
		{
			increase_long(&coder->coder_mutex, &coder->debug_count);
			print_data(DEBUGGING, coder);
			updated_usleep(coder->cd, coder->cd->debug_time);
			increase_long(&coder->coder_mutex, &coder->refactor_count);
			print_data(REFACTORING, coder);
			updated_usleep(coder->cd, coder->cd->refactor_time);
		}
	}
	return (NULL);
}

static int	compile_finish(t_coder *coder)
{
	set_long(&coder->coder_mutex, &coder->last_compile_t, get_time(MILLISEC));
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

static int	compile_helper(t_coder *coder)
{
	if (acquire_dongle(coder, coder->left_dongle))
		return (1);
	if (coding_finished(coder->cd))
	{
		release_dongle(coder, coder->left_dongle);
		return (0);
	}
	print_data(TOOK_DONGLE_1, coder);
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
