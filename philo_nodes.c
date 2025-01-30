/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:07:40 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/30 11:14:53 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


static void add_philo_back(t_philo **philos, t_philo *new)
{
    t_philo *temp;
    
    if (*philos == NULL)
    {
        *philos = new;
        return ;
    }
    temp = (*philos);
    while (temp->next != NULL)
        temp = temp->next;
    temp->next = new;
}

static int set_philo_mutexes(t_philo *new)
{
    if (pthread_mutex_init(&new->meal_flag, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&new->time_flag, NULL) != 0)
    {
        pthread_mutex_destroy(&new->meal_flag);
        return (1);
    }
    else if (pthread_mutex_init(&new->fork, NULL) != 0)
    {
        pthread_mutex_destroy(&new->meal_flag);
        pthread_mutex_destroy(&new->time_flag);
        return (1);
    }
    return (0);
}

static t_philo *new_philo(int id, t_data *data)
{
    t_philo *new;

    new = malloc(sizeof(t_philo));
    if (!new)
    {
        error_return("Failed to malloc a philo", &data, 0);
        return (NULL);
    }
    new->id = id;
    new->next = NULL;
    new->thread = 0;
    new->data = data;
    new->eat_time = -1;
    new->meal_count = 0;
    if (set_philo_mutexes(new) == 1)
    {
        free(new);
        error_return("Couldn't init mutex", &data, 0);
        return (NULL);
    }
    return (new);
}

static void premature_philo_free(t_philo **philos, int i)
{
    t_philo *temp;
    t_philo *next;

    temp = *philos;
    while (i >= 1)
    {
        next = temp->next;
        del_philo_mutex(temp);
        free(temp);
        temp = next;
        i--;
    }
    *philos = NULL;
}

int init_philo_nodes(t_data **data)
{
    t_philo *new;
    int i;

    i = 1;
    while (i <= (*data)->number_of_philos)
    {
        new = new_philo(i, (*data));
        if (!new) 
        {
                premature_philo_free(&(*data)->philos, i - 1);
                free_data(data);
                return (1);
        }
        add_philo_back(&(*data)->philos, new);
        i++;
    }
    new->next = (*data)->philos;
    return (0);    
}
