/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_doing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:50:19 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/22 14:01:36 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void philo_msg(char *s, t_philo *philo)
{
    if (death_check(philo->data) == 1)
        return ;
    pthread_mutex_lock(&philo->data->print_flag);
    printf("%ld %d %s\n", time_is() - philo->data->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->data->print_flag);
}

int death_check(t_data *data)
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
void ft_usleep(long millisecs)
{
    usleep(millisecs * 1000);
}