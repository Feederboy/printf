/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matt <maquentr@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 12:05:52 by matt              #+#    #+#             */
/*   Updated: 2021/02/22 00:14:59 by matt             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		ft_strlen(char *s)
{
	int len;

	len = 0;
	if (!s)
		return (0);
	while (*s++)
		len++;
	return (len);
}

char	*ft_strchr(char *str, char c)
{
	if (!str)
		return (NULL);
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	return (NULL);
}

int		ft_putchar(char c)
{
	return write(1, &c, 1);
}

int		ft_putstr(char *str)
{
	int res;

	res = 0;
	while (*str)
	{
		res += ft_putchar(*str);
		str++;
	}
	return (res);
}

int		ft_putstrl(char *str, int len)
{
	int res;
	int i;

	if (!str)
		return (0);
	res = 0;
	i = 0;
	while (str[i] && i < len)
	{
		res += ft_putchar(str[i]);
		i++;
	}
	return (res);
}

int		ft_isdigit(char c)
{
	return (c >= '0' && c <= '9');
}

int		ft_isspace(char c)
{
	return (c >= 9 && c <= 13) || (c == 32);
}

int				ft_atoi(const char *str)
{
	int i;
	int sign;
	int r;
	int count;

	i = 0;
	sign = 1;
	r = 0;
	count = 0;
	while (ft_isspace(str[i]))
		i++;
	while (str[i] == '+' || str[i] == '-')
	{
		if (count >= 1)
			return (0);
		if (str[i++] == '-')
			sign = -1;
		count++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		r = r * 10 + str[i] - '0';
		i++;
	}
	return (r * sign);
}

int		ft_nb_digits(int d)
{
	int res;

	res = 0;
	if (d == -2147483648)
		return (11);
	if (d < 0)
	{
		res++;
		d *= -1;
	}
	while (d / 10)
	{
		res++;
		d /= 10;
	}
	res++;
	return (res);
}

void	init_args(t_args *args)
{
	args->c = 0;
	args->width = 0;
	args->has_width = 0;
	args->prec = 0;
	args->has_prec = 0;
}

char	*read_args(t_args *args, char *itr)
{
	if (!itr || *itr != '%')
		return (itr);
	itr++; //deplace apres le %
	while (*itr)
	{
		init_args(args);
		//widt
		if (ft_isdigit(*itr))
		{
			args->has_width = 1;
			args->width = ft_atoi(itr);
			itr += ft_nb_digits(args->width);
		}
		//.
		if (*itr == '.')
		{
			args->has_prec = 1;
			itr++;
		}
		//prec
		if (ft_isdigit(*itr))
		{
			args->has_prec = 1;
			args->prec = ft_atoi(itr);
			itr += ft_nb_digits(args->prec);
		}
		//dxs
		if (ft_strchr("dxs", *itr))
		{
			args->c = *itr;
			itr++;
			return (itr);
		}

		itr++;
	}
	return (itr);
}

int		ft_put_s(t_args *args, va_list ap)
{
	int width;
	int precision;
	int len;
	char *s;
	int res;

	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	s = va_arg(ap, char *);
	if (!s)
		s = "(null)";
	len = ft_strlen(s);
	if (args->has_prec)
	{
		if (len > precision)
			len = precision;
	}
	res = 0;
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_putstrl(s, len));
}

int		ft_put_d(t_args *args, va_list ap)
{
	int width;
	int precision;
	int d;
	int len;

	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	d = va_arg(ap, int);
	len = ft_nb_digits(d);
	//A COMPLETER

}

int		ft_put_conv(t_args *args, va_list ap)
{
	if (args->c == 's')
		return ft_put_s(args, ap);
	else if (args->c == 'd')
		return ft_put_d(args,ap);
	return (0);
}

int		ft_printf(const char *format, ...)
{
	char *itr;
	int res;
	t_args args;
	va_list ap;

	itr = (char *)format;
	if (!itr)
		return (0);
	res = 0;
	va_start(ap, format);
	while (*itr)
	{
		if (*itr == '%')
		{
			itr = read_args(&args, itr);
			res += ft_put_conv(&args, ap);
			continue;
		}
		res += ft_putchar(*itr);
		itr++;
	}
	va_end(ap);
	return (res);
}

int main()
{
	ft_printf("hoge\n");
	ft_printf("[%s]\n", "hoge");
	ft_printf("[%10.5s]\n", "hoge");
	ft_printf("[%10.2s]\n", "hoge");
	ft_printf("[%3.2s]\n", "hoge");
	ft_printf("[%3.10s]\n", "hoge");
	ft_printf("[%10.0s]\n", "hoge");
	
	ft_printf("[%s]\n", NULL);
	ft_printf("[%10.5s]\n", NULL);
	ft_printf("[%10.2s]\n", NULL);
	ft_printf("[%3.2s]\n", NULL);
	ft_printf("[%3.10s]\n", NULL);
	ft_printf("[%10.0s]\n", NULL);


	ft_printf("\n\n\n");
	printf("%s\n", NULL);
	printf("%3.2s\n", NULL);

	return (0);
}
