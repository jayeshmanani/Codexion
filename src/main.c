/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:09:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/12 15:13:59 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Input is like ./codexion 5 800 200 200 200 2 0 edf

int	main(int argc, char **argv)
{
	t_args	coding_data;

	if (argc == 9)
	{
		parse_input(&coding_data, argv);
		data_init(&coding_data);
		coding_start(&coding_data);
		clean(&coding_data);
	}
	else
	{
		printf("Fix the input: The input is not as desired!");
	}
	return (0);
}
