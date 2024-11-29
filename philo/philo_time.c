//Stdheader

#include "philo.h"

static void *time_manager(void *content)
{
    t_data *data;
    t_philo *temp;
    int i;
    long current_time;

    data = (t_data *)content;
    temp = data->philos;
    i = 1;
    while (1)
    {
        temp = data->philos;
        i = 1;
        while (i <= data->number_of_philos)
        {
            current_time = time_is(data->real_start_time);
            if (temp->started_eating == 0 && (current_time > (data->start_time + data->time_to_die)))
            {
                printf("%ld %d died without eating\n", time_is(data->real_start_time), temp->id);
                pthread_mutex_lock(&data->death_flag);
                data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                return (NULL);
            }
            if ((temp->started_eating > 0) && (current_time > (temp->eat_time + data->time_to_die)))
            {
                printf("%ld %d died of starvation should have eaten latest at %ld\n", time_is(data->real_start_time), temp->id, (temp->eat_time + data->time_to_die));
                pthread_mutex_lock(&data->death_flag);
                data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                return (NULL);
            }
            temp = temp->next;
            i++;
        }
        usleep(10);
    }
    return (NULL);
}

static void *philo_doing(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;

    while (1)
    {
        if (death_check(philo->data) == 1)
            break ;
        if (eat(philo) == 1)
            break ;
        if (to_sleep(philo) == 1)
            break ;
        if (think(philo) == 1)
            break ;
    }

    return (NULL);
}

static void threading(t_data *data)
{
    t_philo *temp;
    int i;

    //ft_printlst(data);
    temp = data->philos;
    if (data->number_of_philos == 1)
    {
        pthread_create(&temp->thread, NULL, philo_doing, temp);
        pthread_join(temp->thread, NULL);
    }
    else
    {
        i = 1;
        data->real_start_time = init_time_is(); //is this the beginning of the simulation? Get as accurate start time as possible
        pthread_create(&data->manager_thread, NULL, time_manager, data);
        while (i <= data->number_of_philos) //or times to be able to eat is not reached yet
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
        pthread_join(data->manager_thread, NULL);
    }
}

void    philo_time(t_data *data)
{
    pthread_mutex_init(&data->death_flag, NULL);
    threading(data);
}