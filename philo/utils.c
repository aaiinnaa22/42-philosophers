
#include "philo.h"
//del
void    ft_printlst(t_data *data)
{
    t_philo *temp;
    int i;

    temp = data->philos;
    i = 1;
    while (i <= data->number_of_philos)
    {
        printf("node: %d, last: %d, next: %d\n", temp->id, temp->last->id, temp->next->id);
        temp = temp->next;
        i++;
    }
}

int philo_msg(char *s, t_philo *philo)
{
    pthread_mutex_lock(&philo->data->print_flag);
    if (death_check(philo->data) == 1)
    {
        pthread_mutex_unlock(&philo->data->print_flag);
        return (1);
    }
    printf("%ld %d %s\n", time_is() - philo->data->start_time, philo->id, s);
    pthread_mutex_unlock(&philo->data->print_flag);
    return (0);
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

void syncro_start(long start_time)
{
    while (time_is() < start_time)
        continue ;
}