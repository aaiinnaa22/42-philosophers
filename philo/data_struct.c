/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 13:45:10 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 14:54:25 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	set_data_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->death_flag, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&data->print_flag, NULL) != 0)
	{
		pthread_mutex_destroy(&data->death_flag);
		return (1);
	}
	return (0);
}

int	set_data_struct(int ac, char **av, t_data **data)
{
	(*data)->number_of_philos = 0;
	(*data)->time_to_die = 0;
	(*data)->time_to_eat = 0;
	(*data)->time_to_sleep = 0;
	(*data)->times_to_eat = -1;
	(*data)->philos = NULL;
	(*data)->start = 0;
	(*data)->manager_thread = 0;
	(*data)->death = 0;
	if (set_data_mutexes(*data) == 1)
	{
		free(data);
		return (error_return("Couldn't init mutex", data, 0));
	}
	if (insert_data(ac, av, *data) == 1)
		return (error_return("Input is invalid", data, 1));
	return (0);
}
