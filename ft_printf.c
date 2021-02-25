/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matt <maquentr@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 12:05:52 by matt              #+#    #+#             */
/*   Updated: 2021/02/25 15:13:39 by maquentr         ###   ########.fr       */
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

static int		get_nb_size(int base, long long n)
{
	int	size;

	if (n < 0)
	{
		n = -n;
		size = 2;
	}
	else
		size = 1;
	while (n / base > 0)
	{
		n /= base;
		size++;
	}
	return (size);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	if (!(res = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1))))
		return (NULL);
	while (s1[i])
	{
		res[j++] = s1[i++];
	}
	i = 0;
	while (s2[i])
	{
		res[j++] = s2[i++];
	}
	res[j] = '\0';
	return (res);
}


static void		getnb(char **result, char *base, long long n, int pos)
{
	if (n < (long long)ft_strlen(base))
		(*result)[pos] = base[n % ft_strlen(base)];
	else
	{
		getnb(result, base, n / ft_strlen(base), pos - 1);
		(*result)[pos] = base[n % ft_strlen(base)];
	}
}

char			*ft_itoa_base(long n, char *base)
{
	char		*result;
	int			size;
	long long	nb;
	int			baselen;

	baselen = ft_strlen(base);
	nb = n;
	size = get_nb_size(baselen, nb);
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	result[size] = '\0';
	if (nb < 0)
	{
		result[0] = '-';
		nb = -nb;
	}
	getnb(&result, base, nb, size - 1);
	return (result);
}

void	init_args(t_args *args)
{
	args->c = 0;
	args->width = 0;
	args->has_width = 0;
	args->prec = -1;
	args->has_prec = 0;
	args->minus = 0;
	args->has_zero = 0;
	args->zero = 0;
	args->has_star_width = 0;
	args->star_width = -1;
	args->has_star_prec = 0;
	args->star_prec = -1;
	args-> res = 0;
	
}

char	*read_args(t_args *args, char *itr, va_list ap)
{
	if (!itr || *itr != '%')
		return (itr);
	itr++;
	while (*itr)
	{
		init_args(args);
		//minus
		if (*itr == '-')
		{
			args->minus = 1;
			itr++;
		}
		// * width
		if (*itr == '*')
		{
			args->has_star_width = 1;
			args->star_width = va_arg(ap, int);
			itr++;
		}
		//zero
		if (*itr == '0')
		{
			args->has_zero = 1;
			args->zero = 1;
			if (args->minus == 1)
			{
				args->has_zero = 0;
				args->zero = 0;
			}
			itr++;
		}
		//widt ---- if there's no *width before
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
		// * prec
		if (*itr == '*')
		{
			args->has_star_prec = 1;
			args->star_prec = va_arg(ap, int);
			itr++;
		}
		//prec  --- if there's no *prec before
		if (ft_isdigit(*itr))
		{
			if (args->has_star_prec == 0)
			{
				args->has_prec = 1;
				args->prec = ft_atoi(itr);
				itr += ft_nb_digits(args->prec);
			}
			else
				itr += ft_nb_digits(ft_atoi(itr));
		}
		//dxs
		if (ft_strchr("cspdiuxX", *itr))
		{
			args->c = *itr;
			itr++;
			return (itr);
		}

		itr++;
	}
	return (itr);
}

int		ft_put_p(t_args *args, va_list ap)
{
	int width;
	int precision;
	int len;
	long s;
	int res;
	char *base;
	char *tmp;

	base = "0123456789abcdef";
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
	s = (long)va_arg(ap, void *);
	tmp = ft_itoa_base(s, base);
	tmp = ft_strjoin("0x", tmp);
	len = ft_strlen(tmp);
	if (args->has_prec)
	{
		if (len > precision)
			len = precision;
	}
	res = 0;
	if (args->minus)
	{
		res += ft_putstrl(tmp, len);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
	}
	return (res + ft_putstrl(tmp, len));
}

int		ft_put_X(t_args *args, va_list ap)
{
	int width;
	int precision;
	int len;
	long s;
	int res;
	char *base;
	char *tmp;

	base = "0123456789ABCDEF";
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
	s = (long)va_arg(ap, void *);
	tmp = ft_itoa_base(s, base);
	len = ft_strlen(tmp);
	printf("TMP1 = %s\n", tmp);
	if (args->has_prec)
	{
		if (len > precision)
			len = precision;
	}
	res = 0;
	if (args->minus)
	{
		printf("TMP2 = %s\n", tmp);
		res += ft_putstrl(tmp, len);
		printf("TMP3 = %s\n", tmp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
	}

	printf("TMP4 = %s\n", tmp);
	return (res + ft_putstr(tmp));
}

int		ft_put_x(t_args *args, va_list ap)
{
	int width;
	int precision;
	int len;
	long s;
	int res;
	char *base;
	char *tmp;

	base = "0123456789abcdef";
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
	s = (long)va_arg(ap, void *);
	tmp = ft_itoa_base(s, base);
	len = ft_strlen(tmp);
	if (args->has_prec)
	{
		if (len > precision)
			len = precision;
	}
	res = 0;
	if (args->minus)
	{
		res += ft_putstrl(tmp, len);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
	}
	return (res + ft_putstrl(tmp, len));
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
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
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
	if (args->minus)
	{
		res += ft_putstrl(s,len);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
	}
	return (res + ft_putstrl(s, len));
}

int		ft_put_c(t_args *args, va_list ap)
{
	int width;
	int res;
	int c;

	c = va_arg(ap, int);
	width = args->has_width ? args->width : 0;
	res = 0;
	if (args->minus == 1)
	{
		res += ft_putchar(c + 0);
		while (width > 1)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		while (width > 1)
		{
			res += ft_putchar(' ');
			width--;
		}
	}
	return (res + ft_putchar(c + 0));
}

int		ft_put_d_zero(int d, int padding)
{
	int res;

	res = 0;
	if (d == -2147483648)
	{
		res += ft_putchar('-');
		while (padding-- > 0)
			res += ft_putchar('0');
		res += ft_putstr("2147483648");
		return (res);
	}
	if (d < 0)
	{
		res += ft_putchar('-');
		d *= -1;
	}
	while (padding-- > 0)
		res += ft_putchar('0');
	if (d / 10)
		res += ft_put_d_zero(d / 10, padding);
	res += ft_putchar((d % 10) + '0');
	return (res);
}
		
//GERER LE ZERO VIA STRUCT AINSI QUE LETOILE POUR LE NOMBRE DE WIDTH
int		ft_put_d(t_args *args, va_list ap)
{
	int width;
	int precision;
	int d;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	

	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
	d = va_arg(ap, int);
	len = ft_nb_digits(d);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_d_zero(d, padding));
}

int		ft_put_i_zero(int d, int padding)
{
	int res;

	res = 0;
	if (d == -2147483648)
	{
		res += ft_putchar('-');
		while (padding-- > 0)
			res += ft_putchar('0');
		res += ft_putstr("2147483648");
		return (res);
	}
	if (d < 0)
	{
		res += ft_putchar('-');
		d *= -1;
	}
	while (padding-- > 0)
		res += ft_putchar('0');
	if (d / 10)
		res += ft_put_d_zero(d / 10, padding);
	res += ft_putchar((d % 10) + '0');
	return (res);
}

int		ft_put_i(t_args *args, va_list ap)
{
	int width;
	int precision;
	int d;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	

	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	width = args->has_star_width ? args->star_width : 0;
	precision = args->has_star_prec ? args->star_prec : 0;
	d = va_arg(ap, int);
	len = ft_nb_digits(d);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_d_zero(d, padding));
}


int		ft_put_conv(t_args *args, va_list ap)
{
	if (args->c == 'c')
		return ft_put_c(args, ap);
	else if (args->c == 's')
		return ft_put_s(args, ap);
	else if (args->c == 'd')
		return ft_put_d(args,ap);
	else if (args->c == 'p')
		return ft_put_p(args, ap);
	else if (args->c == 'i')
		return ft_put_i(args, ap);
//	else if (args->c == 'u')
//		return ft_put_u(args, ap);
	else if (args->c == 'x')
		return ft_put_x(args, ap);
	else if (args->c == 'X')
		return ft_put_X(args, ap);
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
			itr = read_args(&args, itr, ap);
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
	printf("\n\n\n S \n\n\n");
	ft_printf("hoge\n");
	ft_printf("[%s]\n", "hoge");
	ft_printf("[%10.5s]\n", "hoge");
	ft_printf("[%10.2s]\n", "hoge");
	ft_printf("[%3.2s]\n", "hoge");
	ft_printf("[%3.10s]\n", "hoge");
	ft_printf("[%10.0s]\n", "hoge");


	
	printf("C \n\n\n");

	ft_printf("[%c]\n", 'a');
	ft_printf("[%10c]\n", 'a');
	ft_printf("[%3c]\n", 'a');
	ft_printf("[%5c]\n", 'a');
	
	ft_printf("\n\n\nS NULL\n\n");
	
	ft_printf("[%s]\n", NULL);
	ft_printf("[%10.5s]\n", NULL);
	ft_printf("[%10.2s]\n", NULL);
	ft_printf("[%3.2s]\n", NULL);
	ft_printf("[%3.10s]\n", NULL);
	ft_printf("[%10.0s]\n", NULL);


	printf("D \n\n\n");

	ft_printf("[%05d]\n", 0);
	ft_printf("[%05d]\n", -7);
	ft_printf("[%05d]\n", 123456789);
	ft_printf("[%05d]\n", -203435);
	ft_printf("[%05d]\n", -203435);
	ft_printf("[%05d]\n", -203435);
	
	printf("\n\n\nTRUE PRINTF\n\n");
	
	printf("\n\n\n S \n\n\n");
	printf("[%s]\n", "hoge");
	printf("[%10.5s]\n", "hoge");
	printf("[%10.2s]\n", "hoge");
	printf("[%3.2s]\n", "hoge");
	printf("[%3.10s]\n", "hoge");
	printf("[%10.0s]\n", "hoge");

	printf("\n\n\n C \n\n\n");

	printf("[%c]\n", 'a');
	printf("[%10c]\n", 'a');
	printf("[%3c]\n", 'a');
	printf("[%5c]\n", 'a');
	
	printf("\n\n\nS NULL\n\n");
	
	printf("[%s]\n", NULL);
	printf("[%10.5s]\n", NULL);
	printf("[%10.2s]\n", NULL);
	printf("[%3.2s]\n", NULL);
	printf("[%3.10s]\n", NULL);
	printf("[%10.0s]\n", NULL);

	printf("\n\n\nRANDOM TESTS\n\n");
	
	printf("[%-*.*s]\n", 25, 10 , "jeanmichel");
	printf("[%5.d]\n", 25);
	
	ft_printf("[%8d]\n", -62);
	ft_printf("[%09.12d]\n", 254);
	printf("[%09.12d]\n", 254);

	printf("\n\n\nTEST DDDDDDDD\n\n\n");


	printf("%8d\n", -62);
	ft_printf("%8d\n", -62);
	printf("%.16d\n", -21578);
	ft_printf("%.16d\n", -21578);
	printf("%15ld\n", -2147483648);
	ft_printf("%15d\n", -2147483648);
	printf("%16.13d\n", -9587);
	ft_printf("%16.13d\n", -9587);
	printf("%-.8d\n", -9867);
	ft_printf("%-.8d\n", -9867);
	printf("%09.12d\n", -254);
	ft_printf("%09.12d\n", -254);
	printf("%2.3d\n", 0);
	ft_printf("%2.3d\n", 0);
	printf("%*.*d\n", 5, 3, 12);
	ft_printf("%*.*d\n", 5, 3, 12);


	
	printf("\n\n\nTEST PTR\n\n\n");
	
	
	char	*str = "Ceci est un test.";
	
	ft_printf("%p\n", str);
	printf("%p\n", str);
	ft_printf("%8p\n", str);
	printf("%8p\n", str);
	ft_printf("%16p\n", str);
	printf("%16p\n", str);
	ft_printf("%15p\n", str);
	printf("%15p\n", str);
	ft_printf("%-15p\n", str);
	printf("%-15p\n", str);
	ft_printf("%16p\n", str);
	printf("%16p\n", str);
	ft_printf("%9p\n", str);
	printf("%9p\n", str);
	ft_printf("%p\n", NULL);
	printf("%p\n", NULL);
	ft_printf("%2p\n", NULL);
	printf("%2p\n", NULL);
	ft_printf("%*p\n", 0, NULL);
	printf("%*p\n", 0, NULL);


	printf("\n\n\nTEST HEX\n\n\n");

	printf("%8x\n", -62);
	printf("%8x\n", -62);
	printf("%.16X\n", -21578);
	ft_printf("%.16X\n", -21578);
	printf("%15x\n", -42);
	ft_printf("%15x\n", -42);
	printf("PRINTF[%16.13X]\n", -9587);
	ft_printf("FT[%16.13X]\n", -9587);
	printf("%-.8x\n", -9867);
	ft_printf("%-.8x\n", -9867);
	printf("%09.12X\n", -254);
	ft_printf("%09.12X\n", -254);
	printf("%2.3x\n", 0);
	ft_printf("%2.3x\n", 0);
	printf("TRQLQQ%*.*X\n", 3, 2, 246688);
	ft_printf("TROLOL%*.*X\n", 3, 2, 246688);


	return (0);
}



/*

#include <stdio.h>
#include <unistd.h>
#include <limits.h>

void	char_test(void)
{
	printf("1===%8c.\n", 42);
	ft_printf("1===%8c.\n", 42);
	printf("2===%8c.\n", 42);
	ft_printf("2===%8c.\n", 42);
	printf("3===%88c.\n", 42);
	ft_printf("3===%88c.\n", 42);
	printf("4===%8c.\n", 42);
	ft_printf("4===%8c.\n", 42);
	printf("5===%8c.\n", 42);
	ft_printf("5===%8c.\n", 42);
	printf("6===%-8c.\n", 42);
	ft_printf("6===%-8c.\n", 42);
	printf("7===%8c.\n", 42);
	ft_printf("7===%8c.\n", 42);
	printf("8===%8c.\n", 42);
	ft_printf("8===%8c.\n", 42);
	printf("9===%*c.\n", 0, 42);
	ft_printf("9===%*c.\n", 0, 42);
	printf("10===%c.\n", 42);
	ft_printf("10===%c.\n", 42);
	printf("11==={%3c}\n", -0);
	ft_printf("11==={%3c}\n", -0);
}

void	str_test(void)
{
	char *str = "Ceci est un test.";

	printf("%8s\n", str);
	ft_printf("%8s\n", str);
	printf("%.8s\n", str);
	ft_printf("%.8s\n", str);
	printf("%.0s\n", str);
	ft_printf("%.0s\n", str);
	printf("%8.3s\n", str);
	ft_printf("%8.3s\n", str);
	printf("%-.8s\n", str);
	ft_printf("%-.8s\n", str);
	printf("%9.8s\n", NULL);
	ft_printf("%9.8s\n", NULL);
	printf("%2.3s\n", NULL);
	ft_printf("%2.3s\n", NULL);
	printf("%.*s\n", -3, "hello");
	ft_printf("%.*s\n", -3, "hello");
	printf("%-*.*s\n", -7, -3, str);
	ft_printf("%-*.*s\n", -7, -3, str);
}


void	ptr_test(void)
{
	char	*str = "Ceci est un test.";
;

	printf("%8p\n", str);
	ft_printf("%8p\n", str);
	printf("%16p\n", str);
	ft_printf("%16p\n", str);
	printf("%15p\n", str);
	ft_printf("%15p\n", str);
	printf("%16p\n", str);
	ft_printf("%16p\n", str);
	printf("%-15p\n", NULL);
	ft_printf("%-15p\n", NULL);
	printf("%9p\n", str);
	ft_printf("%9p\n", str);
	printf("%p\n", NULL);
	ft_printf("%p\n", NULL);
	printf("%2p\n", NULL);
	ft_printf("%2p\n", NULL);
	printf("%*p\n", 0, NULL);
	ft_printf("%*p\n", 0, NULL);
}

void	uint_test(void)
{
	printf("%8u\n", 62);
	ft_printf("%8u\n", 62);
	printf("%.16u\n", 21578);
	ft_printf("%.16u\n", 21578);
	printf("%15u\n", 42);
	ft_printf("%15u\n", 42);
	printf("%16.13u\n", 9587);
	ft_printf("%16.13u\n", 9587);
	printf("%-.8u\n", 9867);
	ft_printf("%-.8u\n", 9867);
	printf("%09.12u\n", 254);
	ft_printf("%09.12u\n", 254);
	printf("%2.3u\n", 0);
	ft_printf("%2.3u\n", 0);
	printf("%*.*u\n", 0, 0, 0);
	ft_printf("%*.*u\n", 0, 0, 0);
}
void	int_test(void)
{
	printf("%8d\n", -62);
	ft_printf("%8d\n", -62);
	printf("%.16d\n", -21578);
	ft_printf("%.16d\n", -21578);
	printf("%15d\n", INT_MIN);
	ft_printf("%15d\n", INT_MIN);
	printf("%16.13d\n", -9587);
	ft_printf("%16.13d\n", -9587);
	printf("%-.8d\n", -9867);
	ft_printf("%-.8d\n", -9867);
	printf("%09.12d\n", -254);
	ft_printf("%09.12d\n", -254);
	printf("%2.3d\n", 0);
	ft_printf("%2.3d\n", 0);
	printf("%*.*d\n", 0, 0, 0);
	ft_printf("%*.*d\n", 0, 0, 0);
}
void	hex_test(void)
{
	printf("%8x\n", -62);
	ft_printf("%8x\n", -62);
	printf("%.16X\n", -21578);
	ft_printf("%.16X\n", -21578);
	printf("%15x\n", -42);
	ft_printf("%15x\n", -42);
	printf("%16.13X\n", -9587);
	ft_printf("%16.13X\n", -9587);
	printf("%-.8x\n", -9867);
	ft_printf("%-.8x\n", -9867);
	printf("%09.12X\n", -254);
	ft_printf("%09.12X\n", -254);
	printf("%2.3x\n", 0);
	ft_printf("%2.3x\n", 0);
	printf("%*.*X\n", 0, 0, 0);
	ft_printf("%*.*X\n", 0, 0, 0);
}

void	notype_test(void)
{
	printf("%8%\n");
	ft_printf("%8%\n");
	printf("%.16%\n");
	ft_printf("%.16%\n");
	printf("%15%\n");
	ft_printf("%15%\n");
	printf("%16.13%\n");
	ft_printf("%16.13%\n");
	printf("%-.8%\n");
	ft_printf("%-.8%\n");
	printf("%09.12%\n");
	ft_printf("%09.12%\n");
	printf("%2.3%\n");
	ft_printf("%2.3%\n");
}

void	count_test(void)
{
	int	count, count2;

	printf("oiajwefioja ijargfijg%nfawefaf iuhawef\n", &count);
	ft_printf("oiajwefioja ijargfijg%nfawefaf iuhawef\n", &count2);
	printf("%d68 erte4 6q8e4r %n -> %d\n", count, &count, count);
	ft_printf("%d68 erte4 6q8e4r %n -> %d\n", count2, &count2, count2);
}

void	altflag_test(void)
{
	printf("%#8x\n", -62);
	ft_printf("%#8x\n", -62);
	printf("%#.16X\n", -21578);
	ft_printf("%#.16X\n", -21578);
	printf("%#15x\n", -42);
	ft_printf("%#15x\n", -42);
	printf("%#16.13X\n", -9587);
	ft_printf("%#16.13X\n", -9587);
	printf("%#-.8x\n", -9867);
	ft_printf("%#-.8x\n", -9867);
	printf("%#09.12X\n", -254);
	ft_printf("%#09.12X\n", -254);
	printf("%#2.3x\n", 0);
	ft_printf("%#2.3x\n", 0);
	printf("%#*.*X\n", 0, 0, 0);
	ft_printf("%#*.*X\n", 0, 0, 0);
}

void	sign_test(void)
{
	printf("%+8u\n", 62);
	ft_printf("%+8u\n", 62);
	printf("%+.16u\n", 21578);
	ft_printf("%+.16u\n", 21578);
	printf("%+15u\n", 42);
	ft_printf("%+15u\n", 42);
	printf("%+16.13u\n", 9587);
	ft_printf("%+16.13u\n", 9587);
	printf("%+-.8u\n", 9867);
	ft_printf("%+-.8u\n", 9867);
	printf("%+09.12u\n", 254);
	ft_printf("%+09.12u\n", 254);
	printf("%+2.3u\n", 0);
	ft_printf("%+2.3u\n", 0);
	printf("%+*.*u\n", 0, 0, 0);
	ft_printf("%+*.*u\n", 0, 0, 0);
	printf("%+8i\n", -62);
	ft_printf("%+8i\n", -62);
	printf("%+.16d\n", -21578);
	ft_printf("%+.16d\n", -21578);
	printf("%+15i\n", INT_MIN);
	ft_printf("%+15i\n", INT_MIN);
	printf("%+16.13d\n", -9587);
	ft_printf("%+16.13d\n", -9587);
	printf("%-4i\n", -9867);
	ft_printf("%-4i\n", -9867);
	printf("%+09.12d\n", -254);
	ft_printf("%+09.12d\n", -254);
	printf("%+2.3d\n", 0);
	ft_printf("%+2.3d\n", 0);
	printf("%+*.*i\n", 0, 0, 0);
	ft_printf("%+*.*i\n", 0, 0, 0);
	printf("%+8x\n", -62);
	ft_printf("%+8x\n", -62);
	printf("%+.16X\n", -21578);
	ft_printf("%+.16X\n", -21578);
	printf("%+15x\n", -42);
	ft_printf("%+15x\n", -42);
	printf("%+16.13X\n", -9587);
	ft_printf("%+16.13X\n", -9587);
	printf("%+-.8x\n", -9867);
	ft_printf("%+-.8x\n", -9867);
	printf("%+09.12X\n", -254);
	ft_printf("%+09.12X\n", -254);
	printf("%+2.3x\n", 0);
	ft_printf("%+2.3x\n", 0);
	printf("%+*.*X\n", 0, 0, 0);
	ft_printf("%+*.*X\n", 0, 0, 0);
}

void	blank_test(void)
{
	printf("% 8u\n", 62);
	ft_printf("% 8u\n", 62);
	printf("% .16u\n", 21578);
	ft_printf("% .16u\n", 21578);
	printf("% 15u\n", 42);
	ft_printf("% 15u\n", 42);
	printf("% 16.13u\n", 9587);
	ft_printf("% 16.13u\n", 9587);
	printf("% -.8u\n", 9867);
	ft_printf("% -.8u\n", 9867);
	printf("% 09.12u\n", 254);
	ft_printf("% 09.12u\n", 254);
	printf("% 2.3u\n", 0);
	ft_printf("% 2.3u\n", 0);
	printf("% *.*u\n", 0, 0, 0);
	ft_printf("% *.*u\n", 0, 0, 0);
	printf("% 8i\n", -62);
	ft_printf("% 8i\n", -62);
	printf("% .16d\n", 21578);
	ft_printf("% .16d\n", 21578);
	printf("% 15i\n", INT_MIN);
	ft_printf("% 15i\n", INT_MIN);
	printf("% 16.13d\n", 9587);
	ft_printf("% 16.13d\n", 9587);
	printf("% -4i\n", -9867);
	ft_printf("% -4i\n", -9867);
	printf("% 09.12d\n", 254);
	ft_printf("% 09.12d\n", 254);
	printf("% 2.3d\n", 0);
	ft_printf("% 2.3d\n", 0);
	printf("% *.*i\n", 0, 0, 0);
	ft_printf("% *.*i\n", 0, 0, 0);
	printf("% 8x\n", -62);
	ft_printf("% 8x\n", -62);
	printf("% .16X\n", -21578);
	ft_printf("% .16X\n", -21578);
	printf("% 15x\n", -42);
	ft_printf("% 15x\n", -42);
	printf("% 16.13X\n", -9587);
	ft_printf("% 16.13X\n", -9587);
	printf("% -.8x\n", -9867);
	ft_printf("% -.8x\n", -9867);
	printf("% 09.12X\n", -254);
	ft_printf("% 09.12X\n", -254);
	printf("% 2.3x\n", 0);
	ft_printf("% 2.3x\n", 0);
	printf("% *.*X\n", 0, 0, 0);
	ft_printf("% *.*X\n", 0, 0, 0);
}

void	float_test(void)
{
	printf("%-8f\n", 0.7698765);
	ft_printf("%-8f\n", 0.765);
	printf("%+12.4f\n", 12.75);
	ft_printf("%+12.4f\n", 12.75);
	printf("%3.7f\n", -8766.54123);
	ft_printf("%3.7f\n", -8766.54123);
	printf("%010.7f\n", 8766.54123);
	ft_printf("%010.7f\n", 8766.54123);
	printf("%9.1f\n", -0.231);
	ft_printf("%9.1f\n", -0.231);
	printf("%-9f\n", 0.231);
	ft_printf("%-9f\n", 0.231);
	printf("% 2.3f\n", 55.7655);
	ft_printf("% 2.3f\n", 55.7655);
	printf("%#8.8f\n", -7652.123456);
	ft_printf("%#8.8f\n", -7652.123456);
	printf("%0.0f\n", 7652.123456);
	ft_printf("%0.0f\n", 7652.123456);
	printf("%.4f\n", 0.231);
	ft_printf("%.4f\n", 0.231);
}

void	lmod_test(void)
{
	printf("%8lu\n", 62);
	ft_printf("%8lu\n", 62);
	printf("%.16hu\n", 21578);
	ft_printf("%.16hu\n", 21578);
	printf("%15llu\n", 42);
	ft_printf("%15llu\n", 42);
	printf("%16.13hhu\n", 9587);
	ft_printf("%16.13hhu\n", 9587);
	printf("%-.8hu\n", 9867);
	ft_printf("%-.8hu\n", 9867);
	printf("% 09.12llu\n", 254);
	ft_printf("% 09.12llu\n", 254);
	printf("%2.3llu\n", 0);
	ft_printf("%2.3llu\n", 0);
	printf("% *.*hhu\n", 0, 0, 0);
	ft_printf("% *.*hhu\n", 0, 0, 0);
	printf("% 8hhi\n", -62);
	ft_printf("% 8hhi\n", -62);
	printf("% .16hhd\n", 21578);
	ft_printf("% .16hhd\n", 21578);
	printf("% 15hhi\n", INT_MIN);
	ft_printf("% 15hhi\n", INT_MIN);
	printf("% 16.13lld\n", 9587);
	ft_printf("% 16.13lld\n", 9587);
	printf("% -4li\n", -9867);
	ft_printf("% -4li\n", -9867);
	printf("% 09.12hd\n", 254);
	ft_printf("% 09.12hd\n", 254);
	printf("% 2.3ld\n", 0);
	ft_printf("% 2.3ld\n", 0);
	printf("% *.*hi\n", 0, 0, 0);
	ft_printf("% *.*hi\n", 0, 0, 0);
	printf("% 8lx\n", -62);
	ft_printf("% 8lx\n", -62);
	printf("% .16llX\n", -21578);
	ft_printf("% .16llX\n", -21578);
	printf("% 15hhx\n", -42);
	ft_printf("% 15hhx\n", -42);
	printf("% 16.13hX\n", -9587);
	ft_printf("% 16.13hX\n", -9587);
	printf("% -.8hhx\n", -9867);
	ft_printf("% -.8hhx\n", -9867);
	printf("% 09.12lX\n", -254);
	ft_printf("% 09.12lX\n", -254);
	printf("% 2.3hx\n", 0);
	ft_printf("% 2.3hx\n", 0);
	printf("% *.*hX\n", 0, 0, 0);
	ft_printf("% *.*hX\n", 0, 0, 0);
	printf("%hc\n", 42);
	ft_printf("%hc\n", 42);
	printf("%lc\n", 4245);
	ft_printf("%lc\n", 4245);
	printf("%lc\n", 4278);
	ft_printf("%lc\n", 4278);
	printf("%hc\n", 4276);
	ft_printf("%hc\n", 4276);
	printf("%lc\n", 42123);
	ft_printf("%lc\n", 42123);
	printf("%-hc\n", 42);
	ft_printf("%-hc\n", 42);
	printf("%hc\n", 4209);
	ft_printf("%hc\n", 4209);
	printf("%lc\n", 1242);
	ft_printf("%lc\n", 1242);
	printf("%lc\n", 0, 9042);
	ft_printf("%lc\n", 0, 9042);
	printf("%hc\n", 4);
	ft_printf("%hc\n", 4);
	printf("{%3lc}\n", -0);
	ft_printf("{%3lc}\n", -0);
}
int		main(void)
{
	char_test();
//	str_test();
//	ptr_test();
//	uint_test();
	int_test();
//	hex_test();
//	notype_test();
//	count_test();
//	altflag_test();
//	sign_test();
//	blank_test();
	return (0);
}
*/
