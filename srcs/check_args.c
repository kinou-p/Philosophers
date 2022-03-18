/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/19 00:05:55 by apommier          #+#    #+#             */
/*   Updated: 2022/03/19 00:21:03 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (0);
	while (s[i] != 0)
		i++;
	return (i);
}

int	is_nbr(int nb_args, char **list)
{
	int	j;
	int	i;

	i = 0;
	j = 1;
	while (nb_args - j)
	{
		if (ft_strlen(list[j]) > 10)
			return (0);
		i = 0;
		while (list[j][i])
		{
			if (list[j][i] < '0' || list[j][i] > '9')
				return (0);
			i++;
		}
		if (ft_atoi(list[j]) > 2147483647)
			return (0);
		j++;
	}
	return (1);
}

int	check_arg(int argc, char **argv)
{
	if (argc != 5 && argc != 6)
		return (1);
	if (!is_nbr(argc, argv))
		return (1);
	if (argv[1][0] == '0' || (argv[1][0] == '1' && !argv[1][1]))
		return (1);
	if (argc == 6 && argv[5][0] == '0')
		return (1);
	return (0);
}
