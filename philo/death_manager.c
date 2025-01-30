/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_manager.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:29:56 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 15:24:05 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_done(t_data *data, int philo_id, int status)
{
	pthread_mutex_lock(&data->death_flag);
	data->death = 1;
	pthread_mutex_unlock(&data->death_flag);
	if (status == 1)
	{
		pthread_mutex_lock(&data->print_flag);
		printf(RED "%ld %d died\n" RESET, (time_is() - data->start), philo_id);
		pthread_mutex_unlock(&data->print_flag);
	}
	return (1);
}

static int	enough_eating(t_data *data)
{
	int		i;
	t_philo	*philos;

	philos = data->philos;
	if (data->times_to_eat == -1)
		return (0);
	i = 1;
	while (i <= data->number_of_philos)
	{
		pthread_mutex_lock(&philos->meal_flag);
		if (philos->meal_count < data->times_to_eat)
		{
			pthread_mutex_unlock(&philos->meal_flag);
			return (0);
		}
		pthread_mutex_unlock(&philos->meal_flag);
		i++;
		philos = philos->next;
	}
	return (philo_done(data, 1, 0));
}

static int	has_died(t_data *data)
{
	int		i;
	t_philo	*phi;
	int		die;

	i = 0;
	phi = data->philos;
	die = data->time_to_die;
	while (i <= data->number_of_philos)
	{
		pthread_mutex_lock(&phi->time_flag);
		if ((phi->eat_time == -1) && (time_is() > (data->start + die)))
		{
			pthread_mutex_unlock(&phi->time_flag);
			return (philo_done(data, phi->id, 1));
		}
		else if ((phi->eat_time != -1) && (time_is() > (phi->eat_time + die)))
		{
			pthread_mutex_unlock(&phi->time_flag);
			return (philo_done(data, phi->id, 1));
		}
		pthread_mutex_unlock(&phi->time_flag);
		i++;
		phi = phi->next;
	}
	return (0);
}

void	*death_manager(void *content)
{
	t_data	*data;

	data = (t_data *)content;
	syncro_start(data->start);
	while (1)
	{
		if (enough_eating(data) == 1)
			return (NULL);
		if (has_died(data) == 1)
			return (NULL);
		usleep(10);
	}
	return (NULL);
}
