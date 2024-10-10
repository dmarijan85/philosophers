/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   security.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 12:56:06 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/25 16:22:04 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safemalloc(size_t bytes, t_info *info)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		errexit("Malloc error?!", info);
	return (ret);
}

static void	handle_mutex(int status, t_opcode opcode, t_info *info)
{
	if (0 == status)
		return ;
	if (EINVAL == status && \
		(opcode == LOCK || opcode == UNLOCK || opcode == DESTROY))
		errexit("The value specified by mutex is invalid.", info);
	else if (EINVAL == status && opcode == INIT)
		errexit("The value specified by attr is invalid.", info);
	else if (EDEADLK == status)
		errexit("A deadlock would occur if \
		the thread blocked waiting for mutex.", info);
	else if (EPERM == status)
		errexit("The current thread does not hold a lock on mutex.", info);
	else if (ENOMEM == status)
		errexit("The process cannot allocate \
		enough memory to create another mutex.", info);
	else if (EBUSY == status)
		errexit("Mutex is locked.", info);
}

void	safemutexer(t_mtx *mutex, t_opcode opcode, t_info *info)
{
	if (LOCK == opcode)
		handle_mutex(pthread_mutex_lock(mutex), opcode, info);
	else if (UNLOCK == opcode)
		handle_mutex(pthread_mutex_unlock(mutex), opcode, info);
	else if (INIT == opcode)
		handle_mutex(pthread_mutex_init(mutex, NULL), opcode, info);
	else if (DESTROY == opcode)
		handle_mutex(pthread_mutex_destroy(mutex), opcode, info);
	else
		errexit("Wrong Mutex Operation :(", info);
}

void	handle_thread(int status, t_opcode opcode, t_info *info)
{
	if (status == 0)
		return ;
	else if (status == EAGAIN)
		errexit("No resources to create another thread.", info);
	else if (status == EPERM)
		errexit("The caller does not have appropiate permission.", info);
	else if (status == EINVAL && opcode == CREATE)
		errexit("The value specified by attr is invalid.", info);
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		errexit("Thread is not a joinable thread.", info);
	else if (status == ESRCH)
		errexit("No thread with the ID thread could be found.", info);
	else if (status == EDEADLK)
		errexit("A deadlock was detected or thread specifies the calling \
		thread.", info);
}

void	safethreader(pthread_t *thread, t_opcode opc, t_info *info)
{
	if (opc == JOIN)
		handle_thread(pthread_join(*thread, NULL), opc, info);
	else if (opc == DETACH)
		handle_thread(pthread_detach(*thread), opc, info);
	else
		errexit("Wrong opcode for thread handler.", info);
}
