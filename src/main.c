/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:09:49 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/23 18:33:45 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_coding_data	cd;
	int				status;

	status = 0;
	memset(&cd, 0, sizeof(cd));
	if (argc == 9)
	{
		if (parse_input(&cd, argv))
			return (1);
		if (data_init(&cd))
		{
			clean_all(&cd);
			return (1);
		}
		status = coding_start(&cd);
		clean_all(&cd);
	}
	else
	{
		printf("Fix the input: The input is not as desired!\n");
		printf("Example: ./codexion n_coders t_burnout t_compile ");
		printf("t_debug t_refactor n_compiles cooldown scheduler\n");
	}
	return (status);
}
