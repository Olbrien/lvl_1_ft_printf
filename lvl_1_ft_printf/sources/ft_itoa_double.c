/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_double.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tisantos <tisantos@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 04:00:04 by tisantos          #+#    #+#             */
/*   Updated: 2021/02/06 06:59:59 by tisantos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

static char		*finalize_decimal_with_precision(char *decimal_string,	double n,
											t_slist *slist)
{
	char	*final;
	int		i;
	int		f;
	int		len_decimal;

	i = 0;
	f = 0;
	len_decimal = ft_strlen(decimal_string);

	if(!(final = malloc(sizeof(char) * (len_decimal + slist->precision + 1))))
		return (NULL);
	if (len_decimal > slist->precision)
		i = len_decimal - slist->precision;
	while (len_decimal < slist->precision)
	{
		final[f++] = '0';
		len_decimal++;
		i = 0;
	}
	while(decimal_string[i] != '\0' && i <= slist->precision)
		final[f++] = decimal_string[i++];
	final[f] = '\0';

	//printf("-->%s<--", decimal_string);
	//printf("-->%s<--", final);

	free(decimal_string);
	return (final);
}

static char		*finalize_decimal_no_precision(char *decimal_string,	double n)
{
	char	*final;
	int		i;
	int		f;
	int		len_decimal;

	i = 0;
	f = 0;
	len_decimal = ft_strlen(decimal_string);

	if(!(final = malloc(sizeof(char) * (len_decimal + 7))))
		return (NULL);
	if (len_decimal > 6)
		i = len_decimal - 6;
	while (len_decimal < 6)
	{
		final[f++] = '0';
		len_decimal++;
		i = 0;
	}
	while(decimal_string[i] != '\0' && i <= 6)
		final[f++] = decimal_string[i++];
	final[f] = '\0';

	//printf("-->%s<--", decimal_string);

	free(decimal_string);
	return (final);
}

static char		*finalize_integer(char *integer_string, double n, t_slist *slist)
{
	char	*final;
	int		i;
	int		f;
	int		len_integer;

	i = 0;
	f = 0;
	len_integer = ft_strlen(integer_string);

	if(!(final = malloc(sizeof(char) * (len_integer + 3))))
		return (NULL);
	if (1 / n <= 0 && (int)n == 0 && integer_string[0] != '-')
		final[f++] = '-';
	while(integer_string[i] != '\0')
		final[f++] = integer_string[i++];
	if (slist->precision != 0)
		final[f++] = '.';
	final[f] = '\0';
	free(integer_string);
	return (final);
}


static char		*finalize_process(char *integer_string, char *decimal_string, t_slist *slist,
								double n)
{
	char 	*final;

	integer_string = finalize_integer(integer_string, n, slist);
	if(slist->precision < 0)
		decimal_string = finalize_decimal_no_precision(decimal_string, n);
	else
		decimal_string = finalize_decimal_with_precision(decimal_string, n, slist);
	final = ft_strjoin(integer_string, decimal_string);

	free (decimal_string);
	return (final);
}

static long long		of_power(int ten, int after_point)
{
	long long temp;

	temp = ten;
	if (after_point == 0)
		return (1);
	while (after_point > 1)
	{
		temp *= ten;
		after_point--;
	}
	return (temp);
}

static int			check_bankers_round(double n, t_slist *slist)
{
	double temp;
	long long integer;

	temp = n;
	integer = (long long)n;

	temp = temp - integer;

	if (slist->precision == 0)
		temp = temp * of_power(10, slist->precision + 1);
	else
		temp = temp * of_power(10, slist->precision);

	if (temp < 0)
		temp *= -1;

	if (integer % 2 == 0 && temp == 5)
	{
		return (0);
	}
	return (1);
}

static double		check_precision_condition(double n, t_slist *slist)
{
	long double temp;
	long long check;

	temp = n;

	if (check_bankers_round(n, slist) == 0)
		return (n);

	temp = temp * of_power(10, slist->precision);

	if (n >= 0)
		temp += 0.5;
	else
		temp -= 0.5;
	temp = temp / of_power(10, slist->precision);

	return temp;
}

static long long		get_decimal_number_unsigned(double n, t_slist *slist,
											unsigned long long integer)
{
	double	temp;

	temp = n;
	temp = temp - integer;

	if (slist->precision < 0)
	{
		temp = temp * of_power(10, 6);
		if (n >= 0)
			temp += 0.5;
		else
			temp -= 0.5;
	}
	else
	{
		temp = check_precision_condition(temp, slist);
		temp = temp * of_power(10, slist->precision);
	}

	if ((long long)temp < 0)
		return ((long long)temp * -1);
	else
		return ((long long)temp);
}

static long long		get_decimal_number_signed(double n, t_slist *slist, long long integer)
{
	double	temp;

	temp = n;
	temp = temp - integer;

	if (slist->precision < 0)
	{
		temp = temp * of_power(10, 6);
		if (n >= 0)
			temp += 0.5;
		else
			temp -= 0.5;
	}
	else
	{
		temp = check_precision_condition(temp, slist);
		temp = temp * of_power(10, slist->precision);
	}

	if ((long long)temp < 0)
		return ((long long)temp * -1);
	else
		return ((long long)temp);
}

static long long		get_integer_number(double n, t_slist *slist)
{
	double	temp;

	temp = n;

	if (slist->precision < 0)
	{
		temp = temp * of_power(10, 6);
		if (n >= 0)
			temp += 0.5;
		else
			temp -= 0.5;
		temp = temp / of_power(10, 6);
	}
	else
		temp = check_precision_condition(n, slist);

	return ((long long)temp);
}

static char		*float_process(double n, t_slist *slist)
{
	long long				integer_number;
	char					*integer_string;
	long long				decimal_number;
	char					*decimal_string;
	char					*finalized_string;

	integer_number = get_integer_number(n, slist);
	if (n < 0)
	{
		integer_string = ft_itoa_longlong(integer_number);
		decimal_number = get_decimal_number_signed(n, slist, integer_number);
	}
	else
	{
		integer_string = ft_itoa_unsigned_longlong(integer_number);
		decimal_number = get_decimal_number_unsigned(n, slist, integer_number);
	}
	decimal_string = ft_itoa_unsigned_longlong(decimal_number);

	//printf("integer-->%s<--", integer_string);
	//printf("decimal-->%s<--", decimal_string);

	finalized_string = finalize_process(integer_string, decimal_string, slist, n);

	return (finalized_string);
}


char			*ft_itoa_double(double n, t_slist *slist)
{
	char					*finish;

	finish = float_process(n, slist);

	return (finish);
}