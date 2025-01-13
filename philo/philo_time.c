//Stdheader

#include "philo.h"

static int enough_eating(t_data *data)
{
    int i;
    t_philo *temp;

    if (data->times_to_eat == -1)
        return (0);
    i = 1;
    temp = data->philos;
    while (i <= data->number_of_philos)
    {
        pthread_mutex_lock(&temp->meal_flag);
        if (temp->meal_count < data->times_to_eat)
        {
            pthread_mutex_unlock(&temp->meal_flag);
            return (0);
        }
        pthread_mutex_unlock(&temp->meal_flag);
        i++;
        temp = temp->next;
    }
    return (1);
}
//no death checks for printing here since it takes time??
static void *time_manager(void *content)
{
    t_data *data;
    t_philo *temp;
    int i;

    data = (t_data *)content;
    temp = data->philos;
    i = 1;
    while (1)
    {
        temp = data->philos;
        i = 1;
        if (enough_eating(data) == 1)
        {
            pthread_mutex_lock(&data->death_flag);
            data->death = 1;
            pthread_mutex_unlock(&data->death_flag);
            printf( GREEN "%ld Philosophers have eaten enough\n" RESET, time_is(data->real_start_time));
            return (NULL);
        }
        while (i <= data->number_of_philos)
        {
            pthread_mutex_lock(&temp->meal_flag);
            if (temp->meal_count == 0 && (time_is(data->real_start_time) > (data->start_time + data->time_to_die)))
            {
                pthread_mutex_lock(&data->death_flag);
                data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                pthread_mutex_unlock(&temp->meal_flag);
                printf(RED "%ld %d died without eating\n" RESET, time_is(data->real_start_time), temp->id);
                return (NULL);
            }
            pthread_mutex_lock(&temp->time_flag);
            if ((temp->meal_count > 0) && (time_is(data->real_start_time) > (temp->eat_time + data->time_to_die)))
            {
                pthread_mutex_lock(&data->death_flag);
                data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                printf(RED "%ld %d died of starvation should have eaten latest at %ld\n" RESET, time_is(data->real_start_time), temp->id, (temp->eat_time + data->time_to_die));
                pthread_mutex_unlock(&temp->time_flag);
                pthread_mutex_unlock(&temp->meal_flag);
                return (NULL);
            }
            pthread_mutex_unlock(&temp->time_flag);
            pthread_mutex_unlock(&temp->meal_flag);
            temp = temp->next;
            i++;
        }
        if (death_check(data) == 1)
            return (NULL);
        usleep(1000);
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
        if (death_check(philo->data) == 1)
            break ;
        if (to_sleep(philo) == 1)
            break ;
        if (death_check(philo->data) == 1)
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