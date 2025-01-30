/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 14:48:20 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 15:21:57 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	error_return(char *str, t_data **data, int check)
{
	printf("Error: %s\n", str);
	if (check == 1)
		free_all(data);
	return (1);
}

int	philo_msg(char *s, t_philo *philo)
{
	pthread_mutex_lock(&philo->data->print_flag);
	if (death_check(philo->data) == 1)
	{
		pthread_mutex_unlock(&philo->data->print_flag);
		return (1);
	}
	printf("%ld %d %s\n", time_is() - philo->data->start, philo->id, s);
	pthread_mutex_unlock(&philo->data->print_flag);
	return (0);
}

int	death_check(t_data *data)
{
	pthread_mutex_lock(&data->death_flag);
	if (data->death == 1)
	{
		pthread_mutex_unlock(&data->death_flag);
		return (1);
	}
	pthread_mutex_unlock(&data->death_flag);
	return (0);
}

void	syncro_start(long start_time)
{
	while (time_is() < start_time)
		continue ;
}

long	time_is(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
