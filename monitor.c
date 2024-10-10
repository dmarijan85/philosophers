/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 13:09:45 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:25:57 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	ttdie;

	if (getbool(&philo->philo_mutex, &philo->full, philo->info))
		return (false);
	elapsed = gettime(MILLISECONDS, philo->info) \
		- getlong(&philo->philo_mutex, &philo->lastmealtime, philo->info);
	ttdie = philo->info->ttdie / 1000;
	if (elapsed > ttdie)
		return (true);
	return (false);
}

void	*monitor_dinner(void *data)
{
	t_info	*info;
	int		i;

	info = (t_info *)data;
	while (!allthreadsrunning(&info->table_mutex, \
		&info->threadsrunningnbr, info->max_philos, info))
		;
	while (!simulation_finished(info))
	{
		if (getlong(&info->table_mutex, &info->philosfullnbr, info) \
			== info->max_philos)
			setbool(&info->table_mutex, &info->end_simulation, true, info);
		i = -1;
		while (++i < info->max_philos && !simulation_finished(info))
		{
			if (philo_died(info->philos + i))
			{
				write_status(DIED, info->philos + i, info);
				setbool(&info->table_mutex, &info->end_simulation, true, info);
			}
		}
	}
	return (NULL);
}
