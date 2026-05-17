/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_op.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/14 12:05:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 19:54:34 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_data(t_coder_ops ops, t_coder *coder)
{
	long	elapsed_t;
	long	start_t;

	start_t = get_long(&coder->cd->cd_mutex, &coder->cd->start_coding_t);
	elapsed_t = get_time(MILLISEC) - start_t;
	mutex_safe(&coder->cd->op_mutex, LOCK);
	if ((TOOK_DONGLE_1 == ops || TOOK_DONGLE_2 == ops)
		&& !coding_finished(coder->cd))
		printf("%6ld %3d has taken a dongle\n", elapsed_t, coder->coder_id);
	else if (COMPILING == ops && !coding_finished(coder->cd))
		printf("%6ld %3d is compiling\n", elapsed_t, coder->coder_id);
	else if (DEBUGGING == ops && !coding_finished(coder->cd))
		printf("%6ld %3d is debugging\n", elapsed_t, coder->coder_id);
	else if (REFACTORING == ops && !coding_finished(coder->cd))
		printf("%6ld %3d is refactoring\n", elapsed_t, coder->coder_id);
	else if (BURNED_OUT == ops)
		printf("%6ld %3d burned out\n", elapsed_t, coder->coder_id);
	mutex_safe(&coder->cd->op_mutex, UNLOCK);
}

int	validate_input_reqs(t_coding_data *cd)
{
	if (cd->n_coders <= 0)
		return (1);
	if (cd->burn_time <= 0 || cd->compile_time <= 0 || cd->debug_time < 0
		|| cd->refactor_time < 0 || cd->n_compiles <= 0
		|| cd->cooldown_time < 0)
		return (1);
	return (0);
}
