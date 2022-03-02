/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apommier <apommier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:10:43 by apommier          #+#    #+#             */
/*   Updated: 2022/02/20 19:10:43 by apommier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_arg	*create_data(int argc, char **argv)
{
	t_arg	*data;

	usleep(1500000);
	data = malloc(sizeof(t_arg));
	*data->death = 1;
	data->nb_full_philo = 0;
	data->nb_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat = 0;
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
	return (data);
}

void	exit_philo(t_philo *philo, t_arg *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo - 1)
	{
		pthread_mutex_destroy(philo[i].left_fork);
		free(philo[i].left_fork);
		i++;
	}
	pthread_mutex_destroy(philo[i].left_fork);
	free(philo[i].left_fork);
	pthread_mutex_destroy(philo[i].display);
	free(philo[i].display);
	free(philo);
	free(data);
	exit(1);
}

void	put_event(t_philo philo, char *event)
{
	pthread_mutex_lock(philo.display);
	if (*philo.data->death)
		printf("%ldms Philo N°%d %s\n", get_time() - philo.data->time_start, philo.philo_id, event);
	pthread_mutex_unlock(philo.display);
}

void	philo_take_fork(t_philo philo)
{
	pthread_mutex_lock(philo.left_fork);
	put_event(philo, "has taken a fork");
	pthread_mutex_lock(philo.right_fork);	
	put_event(philo, "has taken a fork");
}

void	philo_eat(t_philo *philo_tmp)
{
	t_philo philo;

	philo = *philo_tmp;
	philo_take_fork(philo);
	*philo_tmp->last_eat = get_time();
	put_event(philo, "is eating");
	usleep(philo.data->time_to_eat * 1000);
	pthread_mutex_unlock(philo.left_fork);
	pthread_mutex_unlock(philo.right_fork);
	put_event(philo, "is sleeping");
	usleep(philo.data->time_to_sleep * 1000);
	put_event(philo, "is thinking");
}

void	*death_checker(t_philo *philo, t_arg *data)
{
	int i;

	i = 0;
	while (*data->death)
	{
		i = -1;
		while (i++ < data->nb_philo - 1 && *data->death && *philo[i].last_eat)
		{
			if ((get_time() - *philo[i].last_eat) > data->time_to_die && philo[i].must_eat)
			{
				*philo[i].data->death = 0;
				pthread_mutex_lock(philo->display);
				printf("%ldms Philo N°%d is died\n",
					get_time() - data->time_start, philo[i].philo_id);
				printf("End: one philo died\n");
				printf("time_to_die: %d TIME: %ld start: %ld philo.start: %ld last_eat: %ld condition: %ld\n", data->time_to_die ,get_time(), data->time_start , philo[i].data->time_start, get_time() - *philo[i].last_eat ,get_time() - *philo[i].last_eat);
				exit_philo(philo, data);
			}
			else if (data->nb_full_philo == data->nb_philo)
			{
				pthread_mutex_lock(philo->display);
				printf("End: all philo are full\n");
				exit_philo(philo, data);
			}
		}
	}
	return (0);
}

void	*routine(void *tmp)
{
	t_philo *philo;

	philo = (t_philo *)tmp;
	*philo->last_eat = get_time();
	if (philo->philo_id % 2)
		usleep(15000);
	while (*philo->data->death && philo->must_eat
		&& philo->data->nb_full_philo != philo->data->nb_philo)
	{
		philo_eat(philo);
		philo->must_eat--;
		if (!philo->must_eat)
			philo->data->nb_full_philo++;
	}
	printf("philo n%d return\n", philo->philo_id);
	return (0);
}

t_philo *create_philo(t_arg *data)
{
	int i;
	t_philo *philo;
	pthread_mutex_t	*display;

	i = 0;
	philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!philo)
		return (0);
	display = malloc(sizeof(pthread_mutex_t));
	if (!display)
		return (0);
	pthread_mutex_init(display, 0);
	while (i < data->nb_philo)
	{
		*philo[i].last_eat = 0; 
		philo[i].display = display;
		philo[i].philo_id = i + 1;
		philo[i].data = data;
		philo[i].must_eat = data->must_eat;
		philo[i].left_fork = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo[i].left_fork, 0);
		i++;
	}
	i = 0;
	while (i < data->nb_philo)
	{
		philo[i].right_fork = philo[(i + 1) % data->nb_philo].left_fork;
		i++;
	}
	return (philo);
}

int	start_philo(t_philo *philo, t_arg *data)
{
	int i;

	i = 0;
	data->time_start = get_time();
	while (i < data->nb_philo)
	{
		pthread_create(&philo[i].thread, 0, &routine, &philo[i]);
		usleep(10);
		i++;
	}
	death_checker(philo, data);
	i = 0;
	while (i < data->nb_philo)
	{
		pthread_join(philo[i++].thread, 0);
	}
	return (0);
}

int check_arg(int argc, char **argv)
{
	int i;
	int j;

	j= 0;
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
	return (0);
}

int main(int argc, char **argv)
{
	t_arg *data;
	t_philo *philo;
	
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



	//put_event(*philo, "starting");
	//if ( philo->philo_id == 3)
	//	*philo->data->test = 0;
	//printf("je suis le numero %d\n", philo->philo_id);
	//printf("test = %d\n", *philo->data->test);