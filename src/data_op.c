/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:05:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 16:30:37 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	write_ops_debug(t_coder_ops ops, t_coder *coder, long elapsed_t)
{
	if (TOOK_DONGLE_1 == ops && !coding_finished(coder->cd))
		printf("%ld %d has taken a dongle 1 - id: %d\n", elapsed_t,
			coder->coder_id, coder->left_dongle->dongle_id);
	else if (TOOK_DONGLE_2 == ops && !coding_finished(coder->cd))
		printf("%ld %d has taken a dongle 2 - id: %d\n", elapsed_t,
			coder->coder_id, coder->right_dongle->dongle_id);
	else if (COMPILING == ops && !coding_finished(coder->cd))
		printf("%ld %d is compiling %ld/%ld\n", elapsed_t, coder->coder_id,
			coder->compile_count, coder->cd->n_compiles);
	else if (DEBUGGING == ops && !coding_finished(coder->cd))
		printf("%ld %d is debugging %ld/%ld\n", elapsed_t, coder->coder_id,
			coder->debug_count, coder->cd->n_compiles);
	else if (REFACTORING == ops && !coding_finished(coder->cd))
		printf("%ld %d is refactoring %ld/%ld\n", elapsed_t, coder->coder_id,
			coder->refactor_count, coder->cd->n_compiles);
	else if (BURNED_OUT == ops && !coding_finished(coder->cd))
		printf("%ld %d burned out\n", elapsed_t, coder->coder_id);
}

void	print_data(t_coder_ops ops, t_coder *coder, bool debug)
{
	long	elapsed_t;
	long	start_t;

	start_t = get_long(&coder->cd->cd_mutex, &coder->cd->start_coding_t);
	elapsed_t = get_time(MILLISEC) - start_t;
	if (coder->coder_work_done)
		return ;
	mutex_safe(&coder->cd->op_mutex, LOCK);
	if (debug)
		write_ops_debug(ops, coder, elapsed_t);
	else
	{
		if ((TOOK_DONGLE_1 == ops || TOOK_DONGLE_2 == ops)
			&& !coding_finished(coder->cd))
			printf("%ld %d has taken a dongle\n", elapsed_t, coder->coder_id);
		else if (COMPILING == ops && !coding_finished(coder->cd))
			printf("%ld %d is compiling\n", elapsed_t, coder->coder_id);
		else if (DEBUGGING == ops && !coding_finished(coder->cd))
			printf("%ld %d is debugging\n", elapsed_t, coder->coder_id);
		else if (REFACTORING == ops && !coding_finished(coder->cd))
			printf("%ld %d is refactoring\n", elapsed_t, coder->coder_id);
		else if (BURNED_OUT == ops)
			printf("%ld %d burned out\n", elapsed_t, coder->coder_id);
	}
	mutex_safe(&coder->cd->op_mutex, UNLOCK);
}
