/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialiser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:48:27 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 17:59:59 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
right fork = su posicion de "silla" (como un array), ya que las forks 
tambien tienen ids en manera de array.
left fork = su posicion en silla + 1, pero el philo de la ultima silla, tendra 
como left fork la primera. esto tmb se puede conseguir con el calculo de:
(posicion + 1) % numero de philos.
EVEN - ODD fork assignment to prevent "mexican standoffs"
*/
static void	assign_forks(t_philo *philo, t_fork *forks, int pos)
{
	int	max_philos;

	max_philos = philo->info->max_philos;
	philo->first_fork = &forks[(pos + 1) % max_philos];
	philo->second_fork = &forks[pos];
	if ((philo->id % 2) == 0)
	{
		philo->first_fork = &forks[pos];
		philo->second_fork = &forks[(pos + 1) % max_philos];
	}
}

static void	philo_init(t_info *info)
{
	int		pos;
	t_philo	*philo;

	pos = 0;
	while (pos < info->max_philos)
	{
		philo = info->philos + pos;
		philo->id = pos + 1;
		philo->full = false;
		philo->meals_counter = 0;
		philo->info = info;
		safemutexer(&philo->philo_mutex, INIT, info);
		assign_forks(philo, info->forks, pos);
		pos++;
	}
}

void	data_init(t_info *info)
{
	int		i;

	i = 0;
	info->threadsrunningnbr = 0;
	info->all_threads_ready = false;
	info->philosfullnbr = 0;
	info->philos = safemalloc(sizeof(t_philo) * info->max_philos, info);
	info->forks = safemalloc(sizeof(t_fork) * info->max_philos, info);
	while (i < info->max_philos)
	{
		safemutexer(&info->forks[i].fork, INIT, info);
		info->forks[i].fork_id = i;
		i++;
	}
	philo_init(info);
}
