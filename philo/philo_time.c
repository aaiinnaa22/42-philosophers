/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:33 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/21 17:09:36 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo.h"

static int enough_eating(t_data *data)
{
    int i;
    t_philo *temp;
    int times_eaten;

    times_eaten = 0;
    if (data->times_to_eat == -1)
        return (-1);
    i = 1;
    temp = data->philos;
    while (i <= data->number_of_philos)
    {
        pthread_mutex_lock(&temp->meal_flag);
        if (temp->meal_count < data->times_to_eat)
        {
            pthread_mutex_unlock(&temp->meal_flag);
            return (-1);
        }
        times_eaten += temp->meal_count;
        pthread_mutex_unlock(&temp->meal_flag);
        i++;
        temp = temp->next;
    }
    return (times_eaten);
}

static void *time_manager(void *content)
{
    t_data *data;
    int res;
    t_philo *temp;
    int i;
    long current_time;

    data = (t_data *)content;
    while (1)
    {
        res = enough_eating(data);
        if (res != -1)
        {
            pthread_mutex_lock(&data->death_flag);
            data->death = 1;
            pthread_mutex_unlock(&data->death_flag);
            printf( GREEN "%ld Philosophers have eaten enough\n" RESET, time_is(data->real_start_time));
            //printf(GREEN "total eat should be: %d, total eat is: %d\n" RESET, data->times_to_eat * data->number_of_philos, res); 
            return (NULL);
        }
        temp = data->philos;
        i = 0;
        //time the same for all? so checking for a certain timestamp?
        while (i <= data->number_of_philos)
        {
            current_time = time_is(data->real_start_time);
            pthread_mutex_lock(&temp->meal_flag);
            pthread_mutex_lock(&temp->time_flag);
            if ((temp->eat_time == -1) && (current_time > (data->start_time + data->time_to_die))) 
            {
                printf("time is:%ld should have eaten: %ld\n", current_time, (data->start_time + data->time_to_die));
                //philo_msg("died without eating", temp);
                pthread_mutex_lock(&data->death_flag);
                temp->data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                pthread_mutex_unlock(&temp->meal_flag);
                pthread_mutex_unlock(&temp->time_flag);
                printf(RED "%ld %d died without eating\n" RESET, current_time, temp->id);
                return (NULL);
            }
            if ((temp->eat_time > -1) && (current_time > (temp->eat_time + data->time_to_die)))
            {
                //philo_msg("died of starvation", temp);
                pthread_mutex_lock(&data->death_flag);
                temp->data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                printf(RED "%ld %d died of starvation should have eaten latest at %ld\n" RESET, current_time, temp->id, (temp->eat_time + data->time_to_die));
                pthread_mutex_unlock(&temp->time_flag);
                pthread_mutex_unlock(&temp->meal_flag);
                return (NULL);
            }
            pthread_mutex_unlock(&temp->time_flag);
            pthread_mutex_unlock(&temp->meal_flag);
            i++;
            temp = temp->next;
        }
        //usleep(2000);
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
            pthread_mutex_init(&temp->meal_flag, NULL);
            pthread_mutex_init(&temp->time_flag, NULL);
            pthread_create(&temp->thread, NULL, philo_doing, temp);
            temp = temp->next;
            i++;
        }

        temp = data->philos;
        i = 1;
        while (i <= data->number_of_philos)
        {
            pthread_join(temp->thread, NULL);
            pthread_mutex_destroy(&temp->fork);
            pthread_mutex_destroy(&temp->meal_flag);
            pthread_mutex_destroy(&temp->time_flag);
            temp = temp->next;
            i++;
        }
        pthread_join(data->manager_thread, NULL);
    }
}

void    philo_time(t_data *data)
{
    //USE PTHREAD MUTEX DESTROY
    pthread_mutex_init(&data->death_flag, NULL);
    pthread_mutex_init(&data->print_flag, NULL);
    threading(data);
    pthread_mutex_destroy(&data->death_flag);
    pthread_mutex_destroy(&data->print_flag);
}
