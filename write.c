/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 12:25:42 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:21:18 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status(t_philo_status status, t_philo *philo, t_info *info)
{
	long	elapsed;

	elapsed = gettime(MILLISECONDS, info) - philo->info->start_time;
	safemutexer(&philo->info->write_mutex, LOCK, info);
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) \
		&& !simulation_finished(info))
		printf("%-6ld %d has taken a fork\n", elapsed, philo->id);
	else if (status == EATING && !simulation_finished(info))
		printf("%-6ld %d is eating\n", elapsed, philo->id);
	else if (status == SLEEPING && !simulation_finished(info))
		printf("%-6ld %d is sleeping\n", elapsed, philo->id);
	else if (status == THINKING && !simulation_finished(info))
		printf("%-6ld %d is thinking\n", elapsed, philo->id);
	else if (status == DIED && !simulation_finished(info))
		printf("%-6ld %d died\n", elapsed, philo->id);
	safemutexer(&philo->info->write_mutex, UNLOCK, info);
}
