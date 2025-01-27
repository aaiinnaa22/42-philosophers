/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 13:24:34 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/27 16:04:35 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> //printf
# include <stdlib.h> //malloc free
# include <pthread.h> //var pthread_t
# include <unistd.h> //usleep
#include <sys/time.h> //gettimeofday

#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

typedef struct s_data   t_data; //I use t_data in t_philo before the computer knows about the struct

typedef struct s_philo //have to mutex everything that can be accessed at the same time? 
                        //like philo->eaten may be accessed in manager while its being changed in another func
{
    int id;
    long    eat_time;
    pthread_t thread;
    pthread_mutex_t fork;
    int meal_count; //test
    pthread_mutex_t meal_flag;
    pthread_mutex_t time_flag;
    struct s_philo *last;
    struct s_philo *next;
    t_data  *data;
} t_philo;

struct s_data //LONG?
{
    int number_of_philos;
    int time_to_die; //time philo will die if doesnt eat
    int time_to_eat; //time it takes for philo to eat
    int time_to_sleep; //time it takes for philo to sleep
    int times_to_eat; //amount of times each philo has to eat !optional
    int death;
    pthread_mutex_t death_flag;
    long    start_time;
    pthread_t manager_thread;
    pthread_mutex_t print_flag;
    t_philo *philos;
};

//handle input
int handle_input(int ac, char **av, t_data *data);

//init
int init_data_struct(t_data *data);
int init_philo_nodes(t_data *data);

//error
int error_return(char *str, t_data **data);
void free_all(t_data **data);
void free_philos(t_philo **philos, int amount);

//philo time
int    philo_time(t_data *data);
int death_check(t_data *data);
int philo_msg(char *s, t_philo *philo);
void ft_usleep(long millisecs);

//del
void    ft_printlst(t_data *data);

//time
long    time_is(void);

#endif