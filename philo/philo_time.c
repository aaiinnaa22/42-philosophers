/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:33 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/22 14:14:31 by aalbrech         ###   ########.fr       */
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

static void *death_manager(void *content)
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
            printf( GREEN "%ld Philosophers have eaten enough\n" RESET, time_is() - data->start_time);
            printf(GREEN "total eat should be: %d, total eat is: %d\n" RESET, data->times_to_eat * data->number_of_philos, res); 
            return (NULL);
        }
        temp = data->philos;
        i = 0;
        while (i <= data->number_of_philos)
        {
            current_time = time_is() - data->start_time;
            pthread_mutex_lock(&temp->meal_flag);
            pthread_mutex_lock(&temp->time_flag);
            if ((temp->eat_time == -1) && (current_time > data->time_to_die)) 
            {
                printf("time is:%ld should have eaten: %d\n", current_time, data->time_to_die);
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
                printf(RED "%ld %d died of starvation should have eaten latest at %ld\n" RESET, current_time, temp->id, (temp->eat_time + data->time_to_die) - data->start_time);
                pthread_mutex_unlock(&temp->time_flag);
                pthread_mutex_unlock(&temp->meal_flag);
                return (NULL);
            }
            pthread_mutex_unlock(&temp->time_flag);
            pthread_mutex_unlock(&temp->meal_flag);
            i++;
            temp = temp->next;
        }
    }
    return (NULL);
}

static void take_forks(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->fork);
        philo_msg("has taken a fork", philo);
        pthread_mutex_lock(&philo->next->fork);
        philo_msg("has taken a fork", philo);
    }
    else
    {
        pthread_mutex_lock(&philo->next->fork);
        philo_msg("has taken a fork", philo);
        pthread_mutex_lock(&philo->fork);
        philo_msg("has taken a fork", philo);
    }
}

static void *philo_doing(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;
    while (death_check(philo->data) == 0)
    {
        philo = (t_philo *)content;
        take_forks(philo);
        philo_msg("is eating", philo);
        pthread_mutex_lock(&philo->time_flag);
        philo->eat_time = time_is();
        pthread_mutex_unlock(&philo->time_flag);
        ft_usleep(philo->data->time_to_eat);
        pthread_mutex_lock(&philo->meal_flag);
        philo->meal_count++;
        pthread_mutex_unlock(&philo->meal_flag);
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
        philo_msg("is sleeping", philo);
        ft_usleep(philo->data->time_to_sleep);
        philo_msg("is thinking", philo);
    }
    return (NULL);
}

static int make_threads(t_data *data)
{
    //make work with only ONE philo
    t_philo *temp;
    int i;

    data->start_time = time_is(); 
    printf("start time is %ld\n", data->start_time);
    temp = data->philos;
    i = 1;
    if (pthread_create(&data->manager_thread, NULL, death_manager, data) != 0)
        return (error_return("Couldn't create thread", &data));
    while (i <= data->number_of_philos)
    {
        if (pthread_create(&temp->thread, NULL, philo_doing, temp) != 0)
            return (error_return("Couldn't create thread", &data));
        temp = temp->next;
        i++;
    }
    return (0);
}
static void end_threads(t_data *data)
{
    t_philo *temp;
    int i;
    
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

int    philo_time(t_data *data)
{
    if (make_threads(data) == 1)
        return (1); //so no, not here immed
    //i have to end threads? and know how many to end
    //if thread == (what i assigned in new_philo), dont join thread
    end_threads(data);
    return (0);
}
