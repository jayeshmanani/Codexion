/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:09:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 20:17:49 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Input is like ./codexion 5 800 200 200 200 2 0 edf

int	main(int argc, char **argv)
{
	t_coding_data	coding_data;

	if (argc == 9)
	{
		if (parse_input(&coding_data, argv))
			return (1);
		data_init(&coding_data);
		coding_start(&coding_data);
		clean_all(&coding_data);
	}
	else
	{
		printf("Fix the input: The input is not as desired!\n");
		printf("Example: ./codexion n_coders t_burnout t_compile ");
		printf("t_debug t_refactor n_compiles cooldown scheduler\n");
	}
	return (0);
}
