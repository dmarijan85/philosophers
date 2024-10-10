/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmarijan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:27:25 by dmarijan          #+#    #+#             */
/*   Updated: 2024/07/30 12:00:59 by dmarijan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static const char	*legaliser(const char *str, t_info *info)
{
	int			len;
	const char	*temp;

	len = 0;
	while (*str == ' ' || *str == '\t' || *str == '\v' || *str == '\n' \
			|| *str == '\r' || *str == '\f')
		str++;
	if (*str == '-')
		errexit("No negative numbers allowed! >:(", info);
	else if (*str == '+')
		str++;
	if (*str < '0' || *str > '9')
		errexit("Only numbers please", info);
	temp = str;
	while (*str >= '0' && *str <= '9')
	{
		str++;
		len++;
	}
	if (len > 10)
		errexit("Number longer than INT MAX", info);
	return (temp);
}

static long	ft_atol(const char *str, t_info *info)
{
	long	re;

	re = 0;
	str = legaliser(str, info);
	while (*str >= '0' && *str <= '9')
	{
		re = (re * 10) + (*str - '0');
		str++;
	}
	if (re > INT_MAX)
		errexit("INT MAX LIMIT", info);
	return (re);
}

//times have to be in microseconds (bc usleep wants it that way) so * 1000
void	parse_input(t_info *info, char **argv)
{
	safemutexer(&info->table_mutex, INIT, info);
	safemutexer(&info->write_mutex, INIT, info);
	info->max_philos = ft_atol(argv[1], info);
	info->ttdie = ft_atol(argv[2], info) * 1000;
	info->tteat = ft_atol(argv[3], info) * 1000;
	info->ttsleep = ft_atol(argv[4], info) * 1000;
	if (info->ttdie < 60000 || info->tteat < 60000 || info->ttsleep < 60000)
		errexit("VALUES TOO SMALL, has to be over 60ms", info);
	if (argv[5])
		info->nbr_limit_meals = ft_atol(argv[5], info);
	else
		info->nbr_limit_meals = -1;
}
