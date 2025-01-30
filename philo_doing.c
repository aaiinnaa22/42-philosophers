/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_doing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:50:19 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 12:07:15 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philo_thinks(t_philo *philo)
{
    long think_time;
    
    if (philo_msg("is thinking", philo) == 1)
        return (1);
    pthread_mutex_lock(&philo->time_flag);
    think_time = (philo->data->time_to_die - (time_is() - philo->eat_time) - philo->data->time_to_eat) / 2;
    pthread_mutex_unlock(&philo->time_flag);
    if (think_time < 0)
        think_time = 0;
    if (think_time == 0)
        think_time = 1;
    if (think_time > 600)
        think_time = 200;
    usleep(think_time * 1000);
    return (0);
}

int philo_eats(t_philo *philo)
{
    if (use_forks(philo, 0) == 1)
        return (1);
    if (philo_msg(YELLOW "is eating" RESET, philo) == 1)
    {
        use_forks(philo, 1);
        return(1);
    }
    pthread_mutex_lock(&philo->time_flag);
    philo->eat_time = time_is();
    pthread_mutex_unlock(&philo->time_flag);
    usleep(philo->data->time_to_eat * 1000);
    pthread_mutex_lock(&philo->meal_flag);
    philo->meal_count++;
    pthread_mutex_unlock(&philo->meal_flag);
    use_forks(philo, 1);
    return (0);
}

int philo_sleeps(t_philo *philo)
{
    if (philo_msg("is sleeping", philo) == 1)
        return (1);
    usleep(philo->data->time_to_sleep * 1000);
    return (0);
}