/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_doing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:50:19 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/16 22:24:07 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*void philo_msg(char *s, t_philo *philo)
{
    //if death, enforce NO printing
    pthread_mutex_lock(&philo->data->print_flag);
    printf("%ld %d %s\n", time_is(philo->data->real_start_time), philo->id, s);
    pthread_mutex_unlock(&philo->data->print_flag);
}*/

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

int in_control(t_philo *philo)  //TEST, so more frequent try if philos are still alive. After some tests: seems to work quite well, but more printing when shouldnt
{
    if (death_check(philo->data) == 1)
        return (1);
    pthread_mutex_lock(&philo->meal_flag);
    if (philo->meal_count == 0 && (time_is(philo->data->real_start_time) > (philo->data->start_time + philo->data->time_to_die)))
    {
        pthread_mutex_lock(&philo->data->death_flag);
        philo->data->death = 1;
        pthread_mutex_unlock(&philo->data->death_flag);
        pthread_mutex_unlock(&philo->meal_flag);
        //philo_msg("died without eating", philo);
        printf(RED "%ld %d died without eating\n" RESET, time_is(philo->data->real_start_time), philo->id);
        return (1);
    }
    pthread_mutex_lock(&philo->time_flag);
    if ((philo->meal_count > 0) && (time_is(philo->data->real_start_time) > (philo->eat_time + philo->data->time_to_die)))
    {
        pthread_mutex_lock(&philo->data->death_flag);
        philo->data->death = 1;
        pthread_mutex_unlock(&philo->data->death_flag);
        printf(RED "%ld %d died of starvation should have eaten latest at %ld\n" RESET, time_is(philo->data->real_start_time), philo->id, (philo->eat_time + philo->data->time_to_die));
        pthread_mutex_unlock(&philo->time_flag);
        pthread_mutex_unlock(&philo->meal_flag);
        return (1);
    }
    pthread_mutex_unlock(&philo->time_flag);
    pthread_mutex_unlock(&philo->meal_flag);
    return (0);
}

int take_forks(t_philo *philo)
{
    if (death_check(philo->data) == 1)
        return (1);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->fork);
        pthread_mutex_lock(&philo->next->fork);
        printf("%ld %d has taken own fork\n", time_is(philo->data->real_start_time), philo->id);
        printf("%ld %d has taken %d fork\n", time_is(philo->data->real_start_time), philo->id, philo->next->id);
    }
    else 
    {
        pthread_mutex_lock(&philo->next->fork);
        pthread_mutex_lock(&philo->fork);
        printf("%ld %d has taken %d fork\n", time_is(philo->data->real_start_time), philo->id, philo->next->id);
        printf("%ld %d has taken own fork\n", time_is(philo->data->real_start_time), philo->id);
    }
    return (0);
}

int    eat(t_philo *philo)
{
    int meal;
    int next_meal;
    int last_meal;
    pthread_mutex_lock(&philo->meal_flag);
    meal = philo->meal_count;
    next_meal = philo->next->meal_count;
    last_meal = philo->last->meal_count;
    pthread_mutex_unlock(&philo->meal_flag);
    if (meal > next_meal) //allowed?
    {
        printf(RED "next can eat says %d:)\n" RESET, philo->id);
        usleep(1000);
    }
    else if (meal > last_meal)
    {
        printf(RED "prev can eat says %d:)\n" RESET, philo->id);
        usleep(1000);
    }
    if (take_forks(philo) == 1)
    {
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
        return (1);
    }
    pthread_mutex_lock(&philo->time_flag);
    philo->eat_time = time_is(philo->data->real_start_time);
    pthread_mutex_unlock(&philo->time_flag);
    if (in_control(philo) == 0 && death_check(philo->data) == 0)
        printf(YELLOW "%ld %d is eating\n" RESET, time_is(philo->data->real_start_time), philo->id);
    else
    {
        pthread_mutex_unlock(&philo->fork);
        pthread_mutex_unlock(&philo->next->fork);
        return (1);
    }
    usleep((philo->data->time_to_eat) * 1000);
    pthread_mutex_unlock(&philo->fork);
    pthread_mutex_unlock(&philo->next->fork);
   pthread_mutex_lock(&philo->meal_flag);
   philo->meal_count++;
   pthread_mutex_unlock(&philo->meal_flag);
    return (0);
}

int    to_sleep(t_philo *philo)
{
    if (in_control(philo) == 0 && death_check(philo->data) == 0)
        printf("%ld %d is sleeping\n", time_is(philo->data->real_start_time), philo->id);
    else
        return (1);
    usleep((philo->data->time_to_sleep) * 1000);
    return (0);
}

int    think(t_philo *philo)
{
    if (in_control(philo) == 0 && death_check(philo->data) == 0)
        printf("%ld %d is thinking\n", time_is(philo->data->real_start_time), philo->id);
    else
        return (1);
    //pthread_mutex_lock(&philo->meal_flag);
    //no improvements made 
    /*if (philo->meal_count >= philo->data->times_to_eat)
    {
        printf(RED "%ld %d SLEEP when thinking\n" RESET, time_is(philo->data->real_start_time), philo->id);
        pthread_mutex_unlock(&philo->meal_flag);
        //usleep(((philo->data->time_to_die) * 1000) - 5000); //-100
        printf(RED "%ld %d STOPPED SLEEP when thinking\n" RESET, time_is(philo->data->real_start_time), philo->id);
    }
    else if ((philo->data->time_to_die + 1) < time_is(philo->data->real_start_time))
    {
        printf(RED "%ld %d sleep when thinking\n" RESET, time_is(philo->data->real_start_time), philo->id);
        usleep(100);
        printf(RED "%ld %d stopped sleep when thinking\n" RESET, time_is(philo->data->real_start_time), philo->id);
    }*/
    //pthread_mutex_unlock(&philo->meal_flag);
    return (0);
}
