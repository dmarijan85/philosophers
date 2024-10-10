/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gsetters.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 12:27:30 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/11 16:44:11 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	setlong(t_mtx *mutex, long *dest, long value, t_info *info)
{
	safemutexer(mutex, LOCK, info);
	*dest = value;
	safemutexer(mutex, UNLOCK, info);
}

long	getlong(t_mtx *mutex, long *value, t_info *info)
{
	long	ret;

	safemutexer(mutex, LOCK, info);
	ret = *value;
	safemutexer(mutex, UNLOCK, info);
	return (ret);
}

void	setbool(t_mtx *mutex, bool *dest, bool value, t_info *info)
{
	safemutexer(mutex, LOCK, info);
	*dest = value;
	safemutexer(mutex, UNLOCK, info);
}

bool	getbool(t_mtx *mutex, bool *value, t_info *info)
{
	bool	ret;

	safemutexer(mutex, LOCK, info);
	ret = *value;
	safemutexer(mutex, UNLOCK, info);
	return (ret);
}

bool	simulation_finished(t_info *info)
{
	return (getbool(&info->table_mutex, &info->end_simulation, info));
}
