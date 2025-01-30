//Stdheader

#include "philo.h"

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

int use_forks(t_philo *philo, int check)
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