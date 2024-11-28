//Stdheader

#include "philo.h"

static void *philo_doing(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;
    eat(philo);
    to_sleep(philo);
    think(philo);

    return (content);
}

static void threading(t_data *data)
{
    t_philo *temp;
    int i;

    ft_printlst(data);
    temp = data->philos;
    if (data->number_of_philos == 1)
    {
        pthread_create(&temp->thread, NULL, philo_doing, temp);
        pthread_join(temp->thread, NULL);
    }
    else
    {
        i = 1;
        while (i <= data->number_of_philos)
        {
            pthread_mutex_init(&temp->fork, NULL);
            pthread_create(&temp->thread, NULL, philo_doing, temp);
            temp = temp->next;
            i++;
        }
        temp = data->philos;
        i = 1;
        while (i <= data->number_of_philos)
        {
            pthread_join(temp->thread, NULL);
            temp = temp->next;
            i++;
        }
    }
}

void    philo_time(t_data *data)
{
    threading(data);
}