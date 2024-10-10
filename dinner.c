/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/09 14:25:08 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:25:41 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *philo, t_info *info, bool flag)
{
	long	tteat;
	long	ttsleep;
	long	ttthink;

	if (!flag)
		write_status(THINKING, philo, info);
	if (philo->info->max_philos % 2 == 0)
		return ;
	tteat = philo->info->tteat;
	ttsleep = philo->info->ttsleep;
	ttthink = (tteat * 2) - ttsleep;
	if (tteat < 0)
		tteat = 0;
	ft_usleep(ttthink * 0.4, info);
}

void	*lone_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_threads(philo->info);
	setlong(&philo->philo_mutex, &philo->lastmealtime, \
		gettime(MILLISECONDS, philo->info), philo->info);
	increase_long(&philo->info->table_mutex, &philo->info->threadsrunningnbr, \
		philo->info);
	write_status(TAKE_FIRST_FORK, philo, philo->info);
	while (!simulation_finished(philo->info))
		usleep(200);
	return (NULL);
}

static void	eat(t_philo *philo, t_info *info)
{
	safemutexer(&philo->first_fork->fork, LOCK, info);
	write_status(TAKE_FIRST_FORK, philo, info);
	safemutexer(&philo->second_fork->fork, LOCK, info);
	write_status(TAKE_SECOND_FORK, philo, info);
	setlong(&philo->philo_mutex, &philo->lastmealtime, \
		gettime(MILLISECONDS, info), info);
	philo->meals_counter++;
	write_status(EATING, philo, info);
	ft_usleep(philo->info->tteat, philo->info);
	if (philo->info->nbr_limit_meals > 0 \
		&& philo->meals_counter == philo->info->nbr_limit_meals)
		setbool(&philo->philo_mutex, &philo->full, true, info);
	safemutexer(&philo->first_fork->fork, UNLOCK, info);
	safemutexer(&philo->second_fork->fork, UNLOCK, info);
}

void	*dinner_sim(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_threads(philo->info);
	setlong(&philo->philo_mutex, &philo->lastmealtime, \
		gettime(MILLISECONDS, philo->info), philo->info);
	increase_long(&philo->info->table_mutex, &philo->info->threadsrunningnbr, \
		philo->info);
	desynchro_philos(philo);
	while (!simulation_finished(philo->info))
	{
		eat(philo, philo->info);
		if (getbool(&philo->philo_mutex, &philo->full, philo->info))
			increase_long(&philo->info->table_mutex, \
				&philo->info->philosfullnbr, philo->info);
		write_status(SLEEPING, philo, philo->info);
		ft_usleep(philo->info->ttsleep, philo->info);
		thinking(philo, philo->info, false);
		if (getbool(&philo->philo_mutex, &philo->full, philo->info))
			break ;
	}
	return (NULL);
}

void	dinner_start(t_info *info)
{
	int	i;

	i = -1;
	if (info->nbr_limit_meals == 0)
		return ;
	else if (info->max_philos == 1)
		handle_thread(pthread_create(&info->philos[0].thread_id, NULL, \
			lone_philo, &info->philos[0]), CREATE, info);
	else
	{
		while (++i < info->max_philos && !simulation_finished(info))
			handle_thread(pthread_create(&info->philos[i].thread_id, NULL, \
				dinner_sim, &info->philos[i]), CREATE, info);
	}
	handle_thread(pthread_create(&info->monitor, NULL, \
		monitor_dinner, info), CREATE, info);
	info->start_time = gettime(MILLISECONDS, info);
	setbool(&info->table_mutex, &info->all_threads_ready, true, info);
	i = -1;
	while (++i < info->max_philos)
		safethreader(&info->philos[i].thread_id, JOIN, info);
	setbool(&info->table_mutex, &info->end_simulation, true, info);
	safethreader(&info->monitor, JOIN, info);
}
