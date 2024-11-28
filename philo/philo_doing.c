//Stdheader

#include "philo.h"

void    eat(t_philo *philo)
{
    pthread_mutex_lock(&philo->next->fork);
    printf("%d has taken a fork\n", philo->id);
    pthread_mutex_lock(&philo->fork);
    printf("%d has taken a fork\n", philo->id);
    printf("%d is eating\n", philo->id);
    usleep(philo->data->time_to_eat);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&philo->next->fork);
}

void    to_sleep(t_philo *philo)
{
    printf("%d is sleeping\n", philo->id);
    usleep(philo->data->time_to_sleep);
}

void    think(t_philo *philo)
{
    printf("%d is thinking\n", philo->id);
}