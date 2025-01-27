/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 15:07:40 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/27 14:59:26 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_data_struct(t_data *data)
{
    data->number_of_philos = 0;
    data->time_to_die = 0;
    data->time_to_eat = 0;
    data->time_to_sleep = 0;
    data->times_to_eat = -1;
    data->philos = NULL;
    data->start_time = 0;
    data->manager_thread = 0;
    data->death = 0;
    if (pthread_mutex_init(&data->death_flag, NULL) != 0)
        return (1);
    if (pthread_mutex_init(&data->print_flag, NULL) != 0)
    {
        pthread_mutex_destroy(&data->death_flag);
        return (1);
    }
    return (0);
}

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

static t_philo *new_philo(int id, t_data *data)
{
    t_philo *new;

    new = malloc(sizeof(t_philo));
    if (!new)
        return (NULL);
    new->id = id;
    new->next = NULL;
    new->last = NULL;
    new->thread = 0;
    if (!data) //not needed prob
    {
        printf("data is null\n"); //FIX
        return (NULL);
    }
    new->data = data;
    new->eat_time = -1;
    new->meal_count = 0;
    if (pthread_mutex_init(&new->meal_flag, NULL) != 0)
    {
        free(new);
        return (NULL);
    }
    if (pthread_mutex_init(&new->time_flag, NULL) != 0)
    {
        pthread_mutex_destroy(&new->meal_flag);
        free(new);
        return (NULL);
    }
    if (pthread_mutex_init(&new->fork, NULL) != 0)
    {
        pthread_mutex_destroy(&new->meal_flag);
        pthread_mutex_destroy(&new->time_flag);
        free(new);
        return (NULL);
    }
    return (new);
}

static void make_lst_circular(t_philo **philos)
{
    t_philo *head;
    t_philo *prev;

    head = *philos;
    prev = NULL;
    while (*philos != NULL)
    {
        if (prev != NULL)
            (*philos)->last = prev;
        prev = *philos;
        *philos = (*philos)->next;
    }
    prev->next = head;
    head->last = prev;
    *philos = head;
}

int init_philo_nodes(t_data *data)
{
    t_philo *new;
    int i;

    i = 1;
    while (i <= data->number_of_philos)
    {
        new = new_philo(i, data);
        if (!new) 
        {
                free_philos(&data->philos, i); //try if works as expected
                return (1);
        }
        add_philo_back(&data->philos, new);
        i++;
    }
    make_lst_circular(&data->philos);
    return (0);    
}
