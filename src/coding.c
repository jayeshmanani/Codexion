/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coding.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 18:19:42 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 18:33:50 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	coding_start(t_coding_data *cd)
{
	if ((NULL == cd) || (0 == cd->n_compiles))
		err_and_exit("Error: coding_data is NULL or 0 Compile needed\n");
}
