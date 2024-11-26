/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:07:40 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/26 16:38:32 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void init_data_struct(t_data *data)
{
    data->number_of_philos = 0;
    data->time_to_die = 0;
    data->time_to_eat = 0;
    data->time_to_sleep = 0;
    data->times_to_eat = 0;
}

int init_philo_node(t_philo *philo)
{
    philo->data = malloc(sizeof(t_data));
    if (!philo->data)
        return (1);
    init_data_struct(philo->data);
    philo->thread = 0;
    return (0);
}
