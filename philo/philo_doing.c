/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_doing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:50:19 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/29 14:26:46 by aalbrech         ###   ########.fr       */
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

static int lock_forks(t_philo *philo, pthread_mutex_t *first, pthread_mutex_t *next)
{
    pthread_mutex_lock(first);
    if (philo_msg("has taken a fork", philo) == 1)
    {
        pthread_mutex_unlock(first);
        return (1);
    }
    pthread_mutex_lock(next);
    if (philo_msg("has taken a fork", philo) == 1)
    {
        pthread_mutex_unlock(next); 
        pthread_mutex_unlock(first);   
        return (1);
    }
    return (0);
}

static void unlock_forks(pthread_mutex_t *first, pthread_mutex_t *next)
{
    pthread_mutex_unlock(first);
    pthread_mutex_unlock(next);
}

static int use_forks(t_philo *philo, int check)
{
    int res;
    
    if (check == 0)
    {
        if (philo->id % 2 == 0)
            res = lock_forks(philo, &philo->fork, &philo->next->fork);
        else
            res = lock_forks(philo, &philo->next->fork, &philo->fork);
        return (res);
    }
    else if (check == 1)
    {
        if (philo->id % 2 == 0)
            unlock_forks(&philo->fork, &philo->next->fork);
        else
            unlock_forks(&philo->next->fork, &philo->fork);
        return (0);
    }
    return (-1);
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
    return (0);
}