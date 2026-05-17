/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/17 14:37:37 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/17 19:48:47 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	destroy_coder(t_coder *coder)
{
	if (!coder)
		return (1);
	if (coder->coder_mutex_initialized)
	{
		if (mutex_safe(&coder->coder_mutex, DESTROY) != 0)
			return (1);
		coder->coder_mutex_initialized = false;
	}
	return (0);
}

int	destroy_all_coders(t_coding_data *cd)
{
	int		i;
	t_coder	*coder;

	i = -1;
	if (!cd || !cd->coders)
		return (1);
	while (++i < cd->n_coders)
	{
		coder = &cd->coders[i];
		if (coder)
		{
			if (destroy_coder(coder))
				return (1);
		}
	}
	free(cd->coders);
	cd->coders = NULL;
	return (0);
}
