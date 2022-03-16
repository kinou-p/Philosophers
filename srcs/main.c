/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 17:46:55 by apommier          #+#    #+#             */
/*   Updated: 2022/03/16 21:57:11 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_arg	*data;
	t_philo	*philo;

	if (check_arg(argc, argv))
	{
		printf("Bad arguments\n");
		return (0);
	}
	data = create_data(argc, argv);
	philo = create_philo(data);
	start_philo(philo, data);
	return (0);
}
