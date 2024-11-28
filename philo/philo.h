/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:24:34 by aalbrech          #+#    #+#             */
/*   Updated: 2024/11/28 13:12:24 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> //printf
# include <stdlib.h> //malloc free


typedef struct s_philo
{
    int id;
    pthread_t thread;
    t_philo *last;
    t_philo *next; 
} t_philo;

typedef struct s_data
{
    int number_of_philos;
    int time_to_die; //time philo will die if doesnt eat
    int time_to_eat; //time it takes for philo to eat
    int time_to_sleep; //time it takes for philo to sleep
    int times_to_eat; //amount of times each philo has to eat !optional
    t_philo *philos;
}   t_data;

//handle input
int handle_input(int ac, char **av, t_data *data);

//init
void init_data_struct(t_data *data);

//error
int error_return(char *str, t_data *data);


#endif