//Stdheader

#include "philo.h"

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

int    eat(t_philo *philo)
{
    if (philo->id < philo->next->id) //prevent deadlocks, that all philos wait for another fork for eternity
    {
        pthread_mutex_lock(&philo->fork);
        printf("%ld %d has taken own fork\n", time_is(philo->data->real_start_time), philo->id);
        pthread_mutex_lock(&philo->next->fork);
        printf("%ld %d has taken %d fork\n", time_is(philo->data->real_start_time), philo->id, philo->next->id);

    }
    else if (philo->id > philo->next->id)
    {
        pthread_mutex_lock(&philo->next->fork);
        printf("%ld %d has taken %d fork\n", time_is(philo->data->real_start_time), philo->id, philo->next->id);
        pthread_mutex_lock(&philo->fork);
        printf("%ld %d has taken own fork\n", time_is(philo->data->real_start_time), philo->id);
    }
    if (death_check(philo->data) == 1)
    {
        pthread_mutex_unlock(&philo->next->fork);
        pthread_mutex_unlock(&philo->fork);
        return (1);
    }
    philo->started_eating++;
    philo->eat_time = time_is(philo->data->real_start_time);
    printf("%ld %d is eating\n", time_is(philo->data->real_start_time), philo->id);
    usleep((philo->data->time_to_eat) * 1000);
    if (philo->id < philo->next->id) //neccesary?
    {
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
    }
    else if (philo->id > philo->next->id)
    {
        pthread_mutex_unlock(&philo->next->fork);
        pthread_mutex_unlock(&philo->fork);
    }
    return (0);
}

int    to_sleep(t_philo *philo)
{
    if (death_check(philo->data) == 1)
        return (1);
    printf("%ld %d is sleeping\n", time_is(philo->data->real_start_time), philo->id);
    usleep((philo->data->time_to_sleep) * 1000);
    return (0);
}

int    think(t_philo *philo)
{
    if (death_check(philo->data) == 1)
        return (1);
    printf("%ld %d is thinking\n", time_is(philo->data->real_start_time), philo->id);
    return (0);
}