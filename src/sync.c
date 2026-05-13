/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 23:35:09 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 23:54:11 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	waiting_for_coders(t_coding_data *cd)
{
	t_mtx	*mtx;
	bool	*coders_ready;

	mtx = &cd->cd_mutex;
	coders_ready = &cd->coders_ready;
	while (!(get_bool(mtx, coders_ready) == false))
		;
}
