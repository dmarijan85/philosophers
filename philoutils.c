/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philoutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:15:33 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:22:38 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean(t_info *info)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < info->max_philos)
	{
		philo = info->philos + i;
		safemutexer(&philo->philo_mutex, DESTROY, info);
		i++;
	}
	safemutexer(&info->write_mutex, DESTROY, info);
	safemutexer(&info->table_mutex, DESTROY, info);
	free(info->forks);
	free(info->philos);
}

/* timecode -> Seconds Milliseconds Microseconds */
long	gettime(t_timecode timecode, t_info *info)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		errexit("Gettime failed :(", info);
		return (0);
	}
	if (timecode == SECONDS)
		return (tv.tv_sec + (tv.tv_usec / 1000000));
	else if (timecode == MILLISECONDS)
		return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
	else if (timecode == MICROSECONDS)
		return ((tv.tv_sec * 1000000) + tv.tv_usec);
	else
		errexit("Wrong input to gettime", info);
	return (0);
}

void	ft_usleep(long usec, t_info *info)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = gettime(MICROSECONDS, info);
	while (gettime(MICROSECONDS, info) - start < usec)
	{
		if (simulation_finished(info))
			break ;
		elapsed = gettime(MICROSECONDS, info) - start;
		remaining = usec - elapsed;
		if (remaining > 1000)
			usleep(remaining / 2);
		else
		{
			while (gettime(MICROSECONDS, info) - start < usec)
				;
		}
	}
}

void	errexit(const char *error, t_info *info)
{
	safemutexer(&info->write_mutex, LOCK, info);
	printf("%s\n", error);
	setbool(&info->table_mutex, &info->end_simulation, true, info);
	safemutexer(&info->write_mutex, UNLOCK, info);
}
