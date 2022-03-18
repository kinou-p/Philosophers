/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 17:40:53 by apommier          #+#    #+#             */
/*   Updated: 2022/03/18 00:09:18 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	exit_philo(t_philo *philo, t_arg *data)
{
	int	i;

	i = 0;
	printf("exit philo\n");
	usleep(100000);
	while (i < data->nb_philo - 1)
	{
		pthread_mutex_destroy(&philo[i].check_nb_eat);
		pthread_mutex_destroy(&philo[i].eat_check);
		pthread_mutex_destroy(philo[i].left_fork);
		free(philo[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(philo[i].left_fork);
	free(philo[i].left_fork);
	pthread_mutex_destroy(data->display);
	pthread_mutex_destroy(&data->life_check);
	free(data->display);
	free(philo);
	free(data);
	exit(1);
}
