/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:31:16 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/29 17:27:31 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *philo_doing(void *content)
{
    t_philo *philo;
    int res;

    philo = (t_philo *)content;
    syncro_start(philo->data->start_time);
    pthread_mutex_lock(&philo->time_flag);
    philo->eat_time = time_is();
    pthread_mutex_unlock(&philo->time_flag);
    if (philo->id % 2 == 0)
        res = philo_thinks(philo);
    else
        res = philo_msg("is thinking",philo);
    if (res == 1)
        return (NULL);
    while (death_check(philo->data) == 0)
    {
        if (philo_eats(philo) == 1)
            return (NULL);
        if (philo_sleeps(philo) == 1)
            return (NULL);
        if (philo_thinks(philo) == 1)
            return (NULL);
    }
    return (NULL);
}

void *one_philo(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;

    pthread_mutex_lock(&philo->fork);
    philo_msg("has taken a fork", philo);
    usleep(philo->data->time_to_die * 1000);
    pthread_mutex_unlock(&philo->fork);
    philo_msg(RED "died" RESET, philo);
    return (NULL); 
}
