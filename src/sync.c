/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 23:35:09 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/14 14:39:55 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	waiting_for_coders(t_coding_data *cd)
{
	while ((get_bool(&cd->cd_mutex, &cd->coders_ready)) == false)
		;
}
