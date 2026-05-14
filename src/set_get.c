/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:16:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 15:00:30 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_bool(t_mtx *mtx, bool *dst, bool value)
{
	mutex_safe(mtx, LOCK);
	*dst = value;
	mutex_safe(mtx, UNLOCK);
}

bool	get_bool(t_mtx *mtx, bool *src)
{
	bool	value;

	mutex_safe(mtx, LOCK);
	value = *src;
	mutex_safe(mtx, UNLOCK);
	return (value);
}

void	set_long(t_mtx *mtx, long *dst, long value)
{
	mutex_safe(mtx, LOCK);
	*dst = value;
	mutex_safe(mtx, UNLOCK);
}

long	get_long(t_mtx *mtx, long *src)
{
	long	value;

	mutex_safe(mtx, LOCK);
	value = *src;
	mutex_safe(mtx, UNLOCK);
	return (value);
}

bool	coding_finished(t_coding_data *cd)
{
	t_mtx	*mtx;
	bool	*end_coding;

	mtx = &cd->cd_mutex;
	end_coding = &cd->end_coding;
	return (get_bool(mtx, end_coding));
}
