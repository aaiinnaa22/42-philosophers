/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_time.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:09:33 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/27 17:34:37 by aalbrech         ###   ########.fr       */
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

static void *death_manager(void *content) //lock orders??!
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
            pthread_mutex_lock(&data->print_flag);
            printf(GREEN "%ld philos have eaten enough\n", time_is() - data->start_time);
            pthread_mutex_unlock(&data->print_flag);
            return (NULL);
        }
        temp = data->philos;
        i = 0;
        while (i <= data->number_of_philos)
        {
            //eat time can vary, 2 % == 0 never is -1, control!
            current_time = time_is() - data->start_time;
            pthread_mutex_lock(&temp->time_flag);
            if ((temp->eat_time == -1) && (current_time > data->time_to_die)) 
            {
                pthread_mutex_lock(&data->death_flag);
                temp->data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                pthread_mutex_lock(&data->print_flag);
                printf(RED "%ld %d died without eating\n" RESET, current_time, temp->id);
                pthread_mutex_unlock(&data->print_flag);
                pthread_mutex_unlock(&temp->time_flag);
                return (NULL);
            }
            if ((temp->eat_time > -1) && (time_is() > (temp->eat_time + data->time_to_die)))
            {
                pthread_mutex_lock(&data->death_flag);
                temp->data->death = 1;
                pthread_mutex_unlock(&data->death_flag);
                pthread_mutex_lock(&data->print_flag);
                printf(RED "%ld %d died of starvation, time %ld is bigger than %ld\n" RESET, (time_is() - data->start_time), temp->id, time_is(), (temp->eat_time + data->time_to_die));
                pthread_mutex_unlock(&data->print_flag);
                pthread_mutex_unlock(&temp->time_flag);
                return (NULL);
            }
            pthread_mutex_unlock(&temp->time_flag);
            i++;
            temp = temp->next;
        }
        usleep(500);
    }
    return (NULL);
}

static int take_forks(t_philo *philo) //add the one philo end
{
    /*if (philo->id == philo->data->number_of_philos) //this makes the simulation fail, but needed for correct lock-order
    {
        pthread_mutex_lock(&philo->next->fork);
        if (philo_msg("has taken a fork", philo) == 1)
        {
            pthread_mutex_unlock(&philo->next->fork);
            return (1);
        }
        pthread_mutex_lock(&philo->fork);
        if (philo_msg("has taken a fork", philo) == 1)
        {
            pthread_mutex_unlock(&philo->fork); 
            pthread_mutex_unlock(&philo->next->fork);   
            return (1);
        }
    }
    else
    {*/
        pthread_mutex_lock(&philo->fork);
        if (philo_msg("has taken a fork", philo) == 1)
        {
            pthread_mutex_unlock(&philo->fork);
            return (1);
        }
        pthread_mutex_lock(&philo->next->fork);
        if (philo_msg("has taken a fork", philo) == 1)
        {
            pthread_mutex_unlock(&philo->next->fork); 
            pthread_mutex_unlock(&philo->fork);   
            return (1);
        }
    //}
    return (0);
}


static int philo_thinks(t_philo *philo)
{
    long think_time;
    
    if (philo_msg("is thinking", philo) == 1)
        return (1);
    pthread_mutex_lock(&philo->time_flag);
    think_time = (philo->data->time_to_die - (time_is() - philo->eat_time) - philo->data->time_to_eat) / 2;
    //pthread_mutex_lock(&philo->data->print_flag);
    //printf("time to die: %d, time since last meal: %ld, time to eat: %d\n", philo->data->time_to_die, (time_is() - philo->eat_time), philo->data->time_to_eat);
   // pthread_mutex_unlock(&philo->data->print_flag);
    pthread_mutex_unlock(&philo->time_flag);
    if (think_time < 0)
        think_time = 0;
    if (think_time == 0)
        think_time = 1;
    if (think_time > 600)
        think_time = 200;
    //pthread_mutex_lock(&philo->data->print_flag);
    //printf("%d is thinking for %ld\n", philo->id, think_time);
    //pthread_mutex_unlock(&philo->data->print_flag);
    ft_usleep(think_time);
    return (0);
}
static void *philo_doing(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->time_flag);
        philo->eat_time = time_is(); //for the think calcu to work
        pthread_mutex_unlock(&philo->time_flag);
        if (philo_thinks(philo) == 1)
            return (NULL);
    }
    while (death_check(philo->data) == 0)
    {
        philo = (t_philo *)content;
        if (take_forks(philo) == 1)
            break ;
        if (philo_msg(YELLOW "is eating" RESET, philo) == 1)
        {
            pthread_mutex_unlock(&philo->fork);
            pthread_mutex_unlock(&philo->next->fork);
            break ;
        }
        pthread_mutex_lock(&philo->time_flag);
        philo->eat_time = time_is(); //time_is() - philo->data->start_time;
        pthread_mutex_unlock(&philo->time_flag);
        ft_usleep(philo->data->time_to_eat);
        pthread_mutex_lock(&philo->meal_flag);
        philo->meal_count++;
        pthread_mutex_unlock(&philo->meal_flag);
        if (philo->id == philo->data->number_of_philos)
        {
            pthread_mutex_unlock(&philo->fork);
            pthread_mutex_unlock(&philo->next->fork);
        }
        else
        {
            pthread_mutex_unlock(&philo->next->fork);
            pthread_mutex_unlock(&philo->fork);
        }
        if (philo_msg("is sleeping", philo) == 1)
            break ;
        ft_usleep(philo->data->time_to_sleep);
        if (philo_thinks(philo) == 1)
            break ;
    }
    return (NULL);
}

static void *one_philo(void *content)
{
    t_philo *philo;

    philo = (t_philo *)content;

    pthread_mutex_lock(&philo->fork);
    philo_msg("has taken a fork", philo);
    ft_usleep(philo->data->time_to_die);
    pthread_mutex_unlock(&philo->fork);
    philo_msg(RED "died" RESET, philo);
    return (NULL); 
}

static int make_threads(t_data *data)
{
    t_philo *temp;
    int i;
    int j;

    data->start_time = time_is(); 
    temp = data->philos;
    i = 1;
    j = 1;
    if (data->number_of_philos == 1)
    {
        if (pthread_create(&temp->thread, NULL, one_philo, temp) != 0)
            return (1);
        pthread_join(temp->thread, NULL);
        return (2);
    }
    if (pthread_create(&data->manager_thread, NULL, death_manager, data) != 0)
        return (1);
    while (i <= data->number_of_philos)
    {
        if ( pthread_create(&temp->thread, NULL, philo_doing, temp) != 0)
        {
            temp = data->philos;
            while (j < i)
            {
                pthread_join(temp->thread, NULL);
                j++;
                temp = temp->next;
            }
            pthread_join(data->manager_thread, NULL);
            return (1);
        }
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
    int res;
    
    res = make_threads(data);
    if (res != 0)
        return (res);
    end_threads(data);
    return (0);
}
