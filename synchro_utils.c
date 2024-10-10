/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 12:37:31 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:26:22 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	desynchro_philos(t_philo *philo)
{
	if (philo->info->max_philos % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(30000, philo->info);
	}
	else
	{
		if (philo->id % 2 == 0)
			thinking(philo, philo->info, true);
	}
}

void	wait_threads(t_info *info)
{
	while (!getbool(&info->table_mutex, &info->all_threads_ready, info))
		;
}

bool	allthreadsrunning(t_mtx *mtx, long *nbrrdy, long phnbr, t_info *info)
{
	bool	ret;

	if (simulation_finished(info))
		return (true);
	ret = false;
	safemutexer(mtx, LOCK, info);
	if (*nbrrdy == phnbr)
		ret = true;
	safemutexer(mtx, UNLOCK, info);
	return (ret);
}

void	*increase_long(t_mtx *mutex, long *value, t_info *info)
{
	safemutexer(mutex, LOCK, info);
	(*value)++;
	safemutexer(mutex, UNLOCK, info);
	return (NULL);
}
