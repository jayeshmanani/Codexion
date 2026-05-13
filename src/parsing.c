/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmanani <jmanani@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 15:14:31 by jmanani           #+#    #+#             */
/*   Updated: 2026/05/13 17:15:25 by jmanani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static inline bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || (32 == c));
}

static const char	*valid_input(const char *s)
{
	int			len;
	const char	*num;

	len = 0;
	while (is_space(*s))
		++s;
	if (*s == '+')
		++s;
	else if (*s == '-')
		err_and_exit("Negative Values Detected: Give only ++++sitive");
	if (!is_digit(*s))
		err_and_exit("Input is not Correct Digit, Update the input");
	num = s;
	while (is_digit(*s++))
		++len;
	if (len > 10)
		err_and_exit("vlaue is not fitting in Int Range");
	return (num);
}

static long	ft_atol(const char *s)
{
	long	num;

	num = 0;
	s = valid_input(s);
	while (is_digit(*s))
		num = (num * 10) + (*s++ - '0');
	if (*s)
		err_and_exit("Invalid input, something other than digit is included");
	if (num > INT_MAX)
		err_and_exit("The Value is too big, No. within INT_MAX is good number");
	return (num);
}

void	parse_input(t_coding_data *cd, char **argv)
{
	cd->n_coders = ft_atol(argv[1]);
	cd->burn_time = ft_atol(argv[2]) * 1e3;
	cd->compile_time = ft_atol(argv[3]) * 1e3;
	cd->debug_time = ft_atol(argv[4]) * 1e3;
	cd->refactor_time = ft_atol(argv[5]) * 1e3;
	cd->n_compiles = ft_atol(argv[6]);
	cd->cooldown_time = ft_atol(argv[7]) * 1e3;
	if (!strcmp(argv[8], "EDF") || !strcmp(argv[8], "edf"))
		cd->scheduler = EDF;
	else if (!strcmp(argv[8], "FIFO") || !strcmp(argv[8], "fifo"))
		cd->scheduler = FIFO;
	else
		err_and_exit("Algorithm can be 'edf' or 'fifo' or 'EDF' or 'FIFO'");
	printf("%ld, %ld, %ld, %ld\n", cd->n_coders, cd->burn_time,
		cd->compile_time, cd->debug_time);
	printf("%ld, %ld, %ld, %d\n", cd->refactor_time, cd->n_compiles,
		cd->cooldown_time, cd->scheduler);
}
