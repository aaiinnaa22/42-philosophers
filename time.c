/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbrech <aalbrech@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:10:01 by aalbrech          #+#    #+#             */
/*   Updated: 2025/01/29 13:58:33 by aalbrech         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "philo.h"

long    time_is(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    //error check?
    return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
