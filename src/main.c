/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:09:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/16 23:14:31 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Input is like ./codexion 5 800 200 200 200 2 0 edf

int	main(int argc, char **argv)
{
	t_coding_data	cd;

	if (argc == 9)
	{
		if (parse_input(&cd, argv))
			return (1);
		if (data_init(&cd))
		{
			free(cd.coders);
			free(cd.dongles);
			heap_destroy(&cd);
			clean_all(&cd);
			return (1);
		}
		printf("Data initialized successfully! Starting the coding simulation...\n");
		int i = 0;
		while(i < cd.n_coders)
		{
			printf("Coder %d: burn_time = %ld, compile_time = %ld, debug_time = %ld, refactor_time = %ld, n_compiles = %ld\n",
				i + 1, cd.burn_time, cd.compile_time,
				cd.debug_time, cd.refactor_time,
				cd.n_compiles);
			printf("Coder -> Dongle ID 1 %d: = %d\n", i + 1, cd.coders[i].left_dongle->dongle_id);
			printf("Coder -> Dongle ID 2 %d: = %d\n", i + 1, cd.coders[i].right_dongle->dongle_id);
			i++;
		}
		coding_start(&cd);
		clean_all(&cd);
	}
	else
	{
		printf("Fix the input: The input is not as desired!\n");
		printf("Example: ./codexion n_coders t_burnout t_compile ");
		printf("t_debug t_refactor n_compiles cooldown scheduler\n");
	}
	return (0);
}
