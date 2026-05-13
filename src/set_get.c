/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:16:48 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 19:21:42 by jmanani          ###   ########.fr       */
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
