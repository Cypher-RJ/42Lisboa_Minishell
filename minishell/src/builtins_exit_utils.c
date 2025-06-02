#include "../includes/minishell.h"

int	ft_ms_atoll(const char *str, long long *nbr)
{
	long long	result;
	int			sign;
	int			digit;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		digit = *str - '0';
		if (sign == 1 && (result > (LLONG_MAX - digit) / 10))
			return (0);
		result = result * 10 + digit;
		str++;
	}
	*nbr = sign * result;
	if (sign == -1 && *nbr > 0)
		return (0);
	return (1);
}

int	is_str_numeric(const char *str)
{
	long long	tmp;
	int			i;

	tmp = 0;
	i = 0;
	if (!str || !str[i])
		return (0);
	while (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (!ft_ms_atoll(str, &tmp))
		return (0);
	return (1);
}
