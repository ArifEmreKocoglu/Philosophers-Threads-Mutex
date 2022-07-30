/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utility.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akocoglu <akocoglu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/30 15:46:26 by akocoglu          #+#    #+#             */
/*   Updated: 2022/07/30 15:47:24 by akocoglu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	cntrl(int nptr)
{
	int	a;
	int	b;

	a = (nptr == '\n' || nptr == '\t' || nptr == '\r');
	b = (nptr == '\v' || nptr == '\f' || nptr == ' ');
	return (a || b);
}

int	ft_atoi(const char *nptr)
{
	long	result;
	int		a;

	while (cntrl(*nptr))
		nptr++;
	a = *nptr == '-';
	nptr += (a || *nptr == '+');
	result = 0;
	while (*nptr >= '0' && *nptr <= '9')
	{
		result = (result * 10) + *nptr++ - '0';
		if (result > 0x7FFFFFFF && a == 0)
			return (-1);
		if (result > 0x80000000 && a == 1)
			return (0);
	}
	return (result * ((a * -2) + 1));
}
