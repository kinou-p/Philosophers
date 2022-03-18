/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/25 03:28:46 by apommier          #+#    #+#             */
/*   Updated: 2022/02/25 03:28:46 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	ft_atoi(const char *nptr)
{
	int		i;
	long	nbr;
	long	minus;

	minus = 1;
	nbr = 0;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
	{
		i++;
		minus = -1;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		nbr = nbr * 10 + nptr[i] - '0';
		i++;
	}
	return (minus * nbr);
}

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	put_event(t_arg *data, int philo_id, char *event)
{
	pthread_mutex_lock(&data->life_check);
	pthread_mutex_lock(data->display);
	if (data->death)
		printf("%ldms Philo N°%d %s\n", get_time() - data->time_start,
			philo_id, event);
	pthread_mutex_unlock(data->display);
	pthread_mutex_unlock(&data->life_check);
}

int	check_arg(int argc, char **argv)
{
	int	i;
	int	j;

	j = 0;
	i = 1;
	if (argc != 5 && argc != 6)
		return (1);
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] > '9' || argv[i][j] < '0')
				return (1);
			j++;
		}
		i++;
	}
	if (argv[1][0] == '0' || (argv[1][0] == '1' && !argv[1][1]))
		return (1);
	if (argc == 6 && argv[5][0] == '0')
		return (1);
	return (0);
}

int	is_philo_full(t_philo *philo, t_arg *data)
{
	int	nb_full;
	int	i;

	nb_full = 0;
	i = -1;
	while (++i < data->nb_philo)
	{
		pthread_mutex_lock(&philo[i].check_nb_eat);
		if (philo[i].nb_eat >= data->must_eat)
			nb_full++;
		pthread_mutex_unlock(&philo[i].check_nb_eat);
	}
	return (nb_full);
}
