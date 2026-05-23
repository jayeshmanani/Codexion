/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_cmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/15 16:00:40 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 13:00:56 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	find_preference(t_req a, t_req b, t_scheduler scheduler)
{
	if (scheduler == EDF)
	{
		if (a.deadline_t != b.deadline_t)
			return (a.deadline_t < b.deadline_t);
		return (a.coder_id < b.coder_id);
	}
	else
	{
		if (a.arrival_t != b.arrival_t)
			return (a.arrival_t < b.arrival_t);
		return (a.coder_id < b.coder_id);
	}
}

// bool	find_preference(t_req a, t_req b, t_scheduler scheduler)
// {
// 	if (scheduler == EDF)
// 	{
//      if (a.deadline_t == b.deadline_t)
// 		return (a.deadline_t < b.deadline_t);
// 	}
//  if (a.arrival_t == b.arrival_t)
// 		return (a.arrival_t < b.arrival_t);
//  return (a.coder_id < b.coder_id);
// }

void	req_swap(t_req *a, t_req *b)
{
	t_req	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

long	heap_parent(long i)
{
	return ((i - 1) / 2);
}

long	heap_left(long i)
{
	return ((i * 2) + 1);
}

long	heap_right(long i)
{
	return ((i * 2) + 2);
}
