/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matt <maquentr@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 12:05:52 by matt              #+#    #+#             */
/*   Updated: 2021/03/17 16:47:20 by maquentr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */




//gerer les prec negatives soit au moment du parsing soit dans la fonction (probablement dans la fonction)



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

int		ft_nb_dig(char *str)
{
	int i;

	i = 0;
	while ((str[i] >= '0' && str[i] <= '9') || str[i] == '-')
		i++;
	return (i);
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

int		ft_check_full_zero(t_args *args, int d)
{
	if ((args->width == -1 && args->prec == 0 && d == 0) || (!args->star_width &&	!args->star_prec && d == 0))
		return (1);
	return (0);
}

int		ft_check_full_zero_long(t_args *args, long d)
{
	if ((args->width == -1 && args->prec == 0 && d == 0) || (!args->star_width &&	!args->star_prec && d == 0))
		return (1);
	return (0);
}

int		ft_ntm(t_args *args, int d) //permet d'afficher %.i avec i = 0
{
	if (!args->has_width && args->has_prec == 1 && args->prec == -1 && d == 0)
		return (1);
	return (0);
}

int		ft_ntm2(t_args *args, long d) //permet d'afficher %.i avec i = 0
{
	if (!args->has_width && args->has_prec == 1 && args->prec == -1 && d == 0)
		return (1);
	return (0);
}
void	init_args(t_args *args)
{
	args->c = 0;
	args->width = -1;
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
	args->percent = 0;

}

char	*read_args(t_args *args, char *itr, va_list ap)
{
	if (!itr || *itr != '%')
		return (itr);
	itr++;
	while (*itr)
	{
		init_args(args);
		//zero
		if (*itr == '0')
		{
			args->zero = 1;
			itr++;
		}
		//minus
		if (*itr == '-')
		{
			args->minus = 1;
			if (args->zero)
				args->zero = 0;
			itr++;
		}
		// * width
		if (*itr == '*')
		{
			args->has_star_width = 1;
			args->star_width = va_arg(ap, int);
			itr++;
		}
		//widt ---- if there's no *width before
		if (ft_isdigit(*itr))
		{
			args->has_width = 1;
			args->width = ft_atoi(itr);
			itr += ft_nb_dig(itr);
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
				args->prec = ft_atoi(itr);
				itr += ft_nb_dig(itr);
			}
			else
				itr += ft_nb_digits(ft_atoi(itr));
		}
		//dxs
		if (ft_strchr("cspdiuxX%", *itr))
		{
			args->c = *itr;
			itr++;
			return (itr);
		}
		itr++;
	}
	return (itr);
}

//--------- PPPPPPP ---------------

int		ft_put_p_zero(char *str, int padding, int fdp)
{
	int res;

	res = 0;
	while (padding-- > 0)
		res += ft_putchar('0');
	if (fdp == 0)
		res += ft_putstr(str);
	else
		res += ft_putchar(' ');
	return (res);
}

int		ft_put_p(t_args *args, va_list ap)
{

	int padding;
	int fdp;
	int width;
	int precision;
	int len;
	long s;
	int res;
	char *base;
	char *tmp;

	fdp = 0;
	base = "0123456789abcdef";
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	res = 0;
	if (args->has_star_width)
		width = args->has_star_width ? args->star_width : 0;
	if (args->has_star_prec)
		precision = args->has_star_prec ? args->star_prec : 0;
	s = (long)va_arg(ap, void *);
	tmp = ft_itoa_base(s, base);
	tmp = ft_strjoin("0x", tmp);
	if (s == 0 && args->has_prec)
		tmp = "0x";
	len = ft_strlen(tmp);
	if (ft_check_full_zero_long(args, s))
		return (0);
	if (ft_ntm2(args, s))
	{
		res +=ft_putstr(tmp);
		return (res);
	}
	if (s < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_p_zero(tmp, padding, fdp);
			return (res);
		}
		else
		{
			if (s < 0)
				padding = (len - 1) < width ? (width - (len)) : 0;
			else
				padding = len < width ? (width - len) : 0;
			res += ft_put_p_zero(tmp, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_p_zero(tmp, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_p_zero(tmp, padding, fdp));
}



// -----------  XXXXXXXX --------------

int		ft_put_X_zero(char *str, int padding, int fdp)
{
	int res;

	res = 0;
	while (padding-- > 0)
		res += ft_putchar('0');
	if (fdp == 0)
		res += ft_putstr(str);
	else
		res += ft_putchar(' ');
	return (res);
}

int		ft_put_X(t_args *args, va_list ap)
{
	int width;
	int precision;
	long d;
	char *tmp;
	char *base;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	int fdp;


	base = "0123456789ABCDEF";
	fdp = 0;
	if (args->has_width && args->has_prec && (args->prec == -1 || args->prec == 0))
		fdp = 1; //pour %5.0i et 5.i   avec i = 0
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	d = (long)va_arg(ap, void*);
	if (width < 0)
	{
		width *= -1;
		args->minus = 1;
	}
	tmp = ft_itoa_base(d, base);
	len = ft_strlen(tmp);
	if (ft_check_full_zero(args, d))
		return (0);
	if (ft_ntm(args, d))
		return (0);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_X_zero(tmp, padding, fdp);
			return (res);
		}
		else
		{
			if (d < 0)
				padding = (len - 1) < width ? (width - (len)) : 0;
			else
				padding = len < width ? (width - len) : 0;
			res += ft_put_X_zero(tmp, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_X_zero(tmp, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_X_zero(tmp, padding, fdp));
}


// ---------- xxxxxxxxxxxx --------------

int		ft_put_x_zero(char *str, int padding, int fdp)
{
	int res;

	res = 0;
	while (padding-- > 0)
		res += ft_putchar('0');
	if (fdp == 0)
		res += ft_putstr(str);
	else
		res += ft_putchar(' ');
	return (res);
}

int		ft_put_x(t_args *args, va_list ap)
{
	int width;
	int precision;
	long d;
	char *tmp;
	char *base;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	int fdp;


	base = "0123456789abcdef";
	fdp = 0;
	if (args->has_width && args->has_prec && (args->prec == -1 || args->prec == 0))
		fdp = 1; //pour %5.0i et 5.i   avec i = 0
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	d = (long)va_arg(ap, void*);
	if (width < 0)
	{
		width *= -1;
		args->minus = 1;
	}
	tmp = ft_itoa_base(d, base);
	len = ft_strlen(tmp);
	if (ft_check_full_zero(args, d))
		return (0);
	if (ft_ntm(args, d))
		return (0);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_x_zero(tmp, padding, fdp);
			return (res);
		}
		else
		{
			if (d < 0)
				padding = (len - 1) < width ? (width - (len)) : 0;
			else
				padding = len < width ? (width - len) : 0;
			res += ft_put_x_zero(tmp, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_x_zero(tmp, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_x_zero(tmp, padding, fdp));
}


// ----------- sssssssss ------------
int		ft_put_s(t_args *args, va_list ap)
{
	int width;
	int precision;
	int len;
	char *s;
	int res;

	res = 0;
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	s = va_arg(ap, char *);
	if (!s)
		s = "(null)";
	len = ft_strlen(s);
	if (args->has_star_width && args->star_width < 0)
	{
		width *= -1;
		res += ft_putstr(s);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	if (args->has_prec && args->has_star_prec && args->star_prec < 0)
		return (res + ft_putstr(s));
	if (args->has_prec)
	{
		if (precision == -1)
			len = 0;
		else if (len > precision)
			len = precision;
	}
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

// ----------- cccccccccc ----------------

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

// ----------- ppppcccccctttttt ----------


int		ft_put_pct(t_args *args, va_list ap)
{
	int width;
	int res;
	(void)ap;

	width = args->has_width ? args->width : 0;
	res = 0;
	if (args->minus == 1)
	{
		res += ft_putchar('%');
		while (width > 1)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	else
	{
		if (args->zero)
		{
			while (width > 1)
			{
				res += ft_putchar('0');
				width--;
			}
		}
		else
		{
			while (width > 1)
			{
				res += ft_putchar(' ');
				width--;
			}
		}
	}
	return (res + ft_putchar('%'));

}

// ------------ uuuuuuuuu -------------

int		ft_put_u_zero(unsigned int d, int padding, int fdp)
{
	int res;

	res = 0;
	if (d == 4294967295)
	{
		while (padding-- > 0)
			res += ft_putchar('0');
		res += ft_putstr("4294967295");
		return (res);
	}
	while (padding-- > 0)
		res += ft_putchar('0');
	if (d / 10)
		res += ft_put_u_zero(d / 10, padding, fdp);
	if (fdp == 0)
		res += ft_putchar((d % 10) + '0');
	else
		res += ft_putchar(' ');
	return (res);
}

int		ft_put_u(t_args *args, va_list ap)
{
	int width;
	int precision;
	unsigned int d;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	int fdp;

	fdp = 0;
	if (args->has_width && args->has_prec && (args->prec == -1 || args->prec == 0))
		fdp = 1; //pour %5.0i et 5.i   avec i = 0
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	d = va_arg(ap, int);
	if (ft_check_full_zero(args, d))
		return (0);
	if (ft_ntm(args, d))
		return (0);
	len = ft_nb_digits(d);
	res = 0;
	padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_u_zero(d, padding, fdp);
			return (res);
		}
		else
		{
			padding = len < width ? (width - len) : 0;
			res += ft_put_u_zero(d, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_u_zero(d, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_u_zero(d, padding, fdp));
}


int		ft_put_d_zero(int d, int padding, int fdp)
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
		res += ft_put_d_zero(d / 10, padding, fdp);
	if (fdp == 0)
		res += ft_putchar((d % 10) + '0');
	else
		res += ft_putchar(' ');
	return (res);
}

int		ft_put_d(t_args *args, va_list ap)
{
	int width;
	int precision;
	int d;
	int len;
	int padding;
	int res; //VARIABLE A METTRE PLUTOT DANS LA STRUCTURE
	int fdp;

	fdp = 0;
	if (args->has_width && args->has_prec && (args->prec == -1 || args->prec == 0))
		fdp = 1; //pour %5.0i et 5.i   avec i = 0
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	d = va_arg(ap, int);
	if (ft_check_full_zero(args, d))
		return (0);
	if (ft_ntm(args, d))
		return (0);
	len = ft_nb_digits(d);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_d_zero(d, padding, fdp);
			return (res);
		}
		else
		{
			if (d < 0)
				padding = (len - 1) < width ? (width - (len)) : 0;
			else
				padding = len < width ? (width - len) : 0;
			res += ft_put_d_zero(d, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_d_zero(d, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_d_zero(d, padding, fdp));
}


// ----------- iiiiiiiiiii ---------

int		ft_put_i_zero(int d, int padding, int fdp)
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
		res += ft_put_i_zero(d / 10, padding, fdp);
	if (fdp == 0)
		res += ft_putchar((d % 10) + '0');
	else
		res += ft_putchar(' ');
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
	int fdp;

	fdp = 0;
	if (args->has_width && args->has_prec && (args->prec == -1 || args->prec == 0))
		fdp = 1; //pour %5.0i et 5.i   avec i = 0
	width = args->has_width ? args->width : 0;
	precision = args->has_prec ? args->prec : 0;
	if (args->has_star_width){
		width = args->has_star_width ? args->star_width : 0;
	}
	if (args->has_star_prec){
		precision = args->has_star_prec ? args->star_prec : 0;
	}
	if (args->star_width < 0 && args->has_prec == 0 && args->has_star_prec == 0 && args->has_width == 0)
	{
		if (args->zero)
			args->zero = 0;
		width *= -1;
		args->minus = 1;
	}
	d = va_arg(ap, int);
	if (ft_check_full_zero(args, d))
		return (0);
	if (ft_ntm(args, d))
		return (0);
	len = ft_nb_digits(d);
	res = 0;
	if (d < 0)
		padding = (len - 1) < precision ? (precision - (len - 1)) : 0;
	else
		padding = len < precision ? (precision - len) : 0;
	len += padding;
	if (args->zero)
	{
		if (args->has_prec)
		{
			while ((width - len) > 0)
			{
				res += ft_putchar(' ');
				width--;
			}
			res += ft_put_i_zero(d, padding, fdp);
			return (res);
		}
		else
		{
			if (d < 0)
				padding = (len - 1) < width ? (width - (len)) : 0;
			else
				padding = len < width ? (width - len) : 0;
			res += ft_put_i_zero(d, padding, fdp);
			return (res);
		}
	}
	if (args->minus)
	{
		res += ft_put_i_zero(d, padding, fdp);
		while ((width - len) > 0)
		{
			res += ft_putchar(' ');
			width--;
		}
		return (res);
	}
	while ((width - len) > 0)
	{
		res += ft_putchar(' ');
		width--;
	}
	return (res + ft_put_i_zero(d, padding, fdp));
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
	else if (args->c == 'u')
		return ft_put_u(args, ap);
	else if (args->c == 'x')
		return ft_put_x(args, ap);
	else if (args->c == 'X')
		return ft_put_X(args, ap);
	else if(args->c == '%')
		return ft_put_pct(args, ap);
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
	printf("[%010.5i]\n", -216);
	ft_printf("[%010.5i]\n", -216);
	printf("[%07i]\n", -216);
	ft_printf("[%07i]\n", -216);
	printf("[%08.5i]\n", 0);
	ft_printf("[%08.5i]\n", 0);
	printf("[%08.3i]\n", 8375);
	ft_printf("[%08.3i]\n", 8375);
	printf("[%0-8.5i]\n", 34);
	ft_printf("[%0-8.5i]\n", 34);
	printf("[%0-10.5i]\n", -216);
	ft_printf("[%0-10.5i]\n", -216);
	printf("[%0-8.5i]\n", 0);
	ft_printf("[%0-8.5i]\n", 0);
	printf("[%.0i]\n", 0);
	ft_printf("[%.0i]\n", 0);
	printf("[%3i]\n", 0);
	ft_printf("[%3i]\n", 0);
	printf("[%i]\n", 0);
	ft_printf("[%i]\n", 0);
	printf("[%3i]\n", 0);
	ft_printf("[%3i]\n", 0);
	printf("[%-3i]\n", 0);
	ft_printf("[%-3i]\n", 0);
	printf("[%.3i]\n", 0);
	ft_printf("[%.3i]\n", 0);
	printf("[%03i]\n", 0);
	ft_printf("[%03i]\n", 0);
	printf("[%8.5i]\n", 0);
	ft_printf("[%8.5i]\n", 0);
	printf("[%.i]\n", 0);
	ft_printf("[%.i]\n", 0);
	printf("[%5.0i]\n", 0);
	ft_printf("[%5.0i]\n", 0);
	printf("[%5.i]\n", 0);
	ft_printf("[%5.i]\n", 0);
	printf("[%u]\n", 4294967295u);
	ft_printf("[%u]\n", 4294967295u);
	printf("[%8.3x]\n", 8375);
	ft_printf("[%8.3x]\n", 8375);
	printf("[%-8.3x]\n", 8375);
	ft_printf("[%-8.3x]\n", 8375);
	printf("[%-3.3x]\n", 6983);
	ft_printf("[%-3.3x]\n", 6983);
	printf("[%08.3x]\n", 8375);
	ft_printf("[%08.3x]\n", 8375);
	printf("[%0.x]\n", 0);
	ft_printf("[%0.x]\n", 0);
	printf("[%5.0x]\n", 0);
	ft_printf("[%5.0x]\n", 0);
	printf("[%7x]\n", 33);
	ft_printf("[%7x]\n", 33);
	printf("[%-7x]\n", 33);
	ft_printf("[%-7x]\n", 33);
	printf("[%.5x]\n", 21);
	ft_printf("[%.5x]\n", 21);
	printf("[%0-8.3x]\n", 8375);
	ft_printf("[%0-8.3x]\n", 8375);
	printf("[%05x]\n", 43);
	ft_printf("[%05x]\n", 43);
	printf("[%03x]\n", 0);
	ft_printf("[%03x]\n", 0);
	printf("[%.p]\n", NULL);
	ft_printf("[%.p]\n", NULL);
	printf("[%*s]\n", -32, "abc");
	ft_printf("[%*s]\n", -32, "abc");
	printf("[%.*s]\n", -3, "hello");
	ft_printf("[%.*s]\n", -3, "hello");
	printf("[%-*.*s]\n", -7, -3, "yolo");
	ft_printf("[%-*.*s]\n", -7, -3, "yolo");
	printf("[%*s]\n", 32, "abc");
	ft_printf("[%*s]\n", 32, "abc");
	printf("[%*i]\n", -14, 94827);
	ft_printf("[%*i]\n", -14, 94827);
	printf("[%*i]\n", -14, -54);
	ft_printf("[%*i]\n", -14, -54);
	printf("[%*x]\n", -14, -12);
	ft_printf("[%*x]\n", -14, -12);
	printf("[%*X]\n", -14, -12);
	ft_printf("[%*X]\n", -14, -12);



	int		a = -4;
	int		b = 0;
	char	c = 'a';
	int		d = 2147483647;
	int		e = -2147483648;
	int		f = 42;
	int		g = 25;
	int		h = 4200;
	int		i = 8;
	int		j = -12;
	int		k = 123456789;
	int		l = 0;
	int		m = -12345678;
	char	*n = "abcdefghijklmnop";
	char	*o = "-a";
	char	*p = "-12";
	char	*q = "0";
	char	*r = "%%";
	char	*s = "-2147483648";
	char	*t = "0x12345678";
	char	*u = "-0";

	printf("Simple input test\n");
	printf("%i, %d, %d, %d, %d, %s, %c, %d, %u, %x, %X\n", i, j, k, l, m, n, c, c, j, j, j); //T2
	ft_printf("%i, %d, %d, %d, %d, %s, %c, %d, %u, %x, %X\n", i, j, k, l, m, n, c, c, j, j, j); //T2
	printf("%0i, %0d, %0d, %0d, %0d, %0d, %0d, %0d\n", i, j, k, l, m, c, e, d); //T3
	ft_printf("%0i, %0d, %0d, %0d, %0d, %0d, %0d, %0d\n", i, j, k, l, m, c, e, d); //T3
	printf("%1i, %1d, %1d, %1d, %1d, %1d, %1d, %1d\n", i, j, k, l, m, c, e, d); //T4
	ft_printf("%1i, %1d, %1d, %1d, %1d, %1d, %1d, %1d\n", i, j, k, l, m, c, e, d); //T4
	printf("%2i, %2d, %2d, %2d, %2d, %2d, %2d, %2d\n", i, j, k, l, m, c, e, d); //T5
	ft_printf("%2i, %2d, %2d, %2d, %2d, %2d, %2d, %2d\n", i, j, k, l, m, c, e, d); //T5
	printf("%3i, %3d, %3d, %3d, %3d, %3d, %3d, %3d\n", i, j, k, l, m, c, e, d); //T6
	ft_printf("%3i, %3d, %3d, %3d, %3d, %3d, %3d, %3d\n", i, j, k, l, m, c, e, d); //T6
	printf("%4i, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n", i, j, k, l, m, c, e, d); //T7
	ft_printf("%4i, %4d, %4d, %4d, %4d, %4d, %4d, %4d\n", i, j, k, l, m, c, e, d); //T7
	printf("%5i, %5d, %5d, %5d, %5d, %5d, %5d, %5d\n", i, j, k, l, m, c, e, d); //T8
	ft_printf("%5i, %5d, %5d, %5d, %5d, %5d, %5d, %5d\n", i, j, k, l, m, c, e, d); //T8
	printf("%6i, %6d, %6d, %6d, %6d, %6d, %6d, %6d\n", i, j, k, l, m, c, e, d); //T9
	ft_printf("%6i, %6d, %6d, %6d, %6d, %6d, %6d, %6d\n", i, j, k, l, m, c, e, d); //T9
	printf("%7i, %7d, %7d, %7d, %7d, %7d, %7d, %7d\n", i, j, k, l, m, c, e, d); //T10
	printf("%7i, %7d, %7d, %7d, %7d, %7d, %7d, %7d\n", i, j, k, l, m, c, e, d); //T10
	ft_printf("%7i, %7d, %7d, %7d, %7d, %7d, %7d, %7d\n", i, j, k, l, m, c, e, d); //T10
	printf("%8i, %8d, %8d, %8d, %8d, %8d, %8d, %8d\n", i, j, k, l, m, c, e, d); //T11
	ft_printf("%8i, %8d, %8d, %8d, %8d, %8d, %8d, %8d\n", i, j, k, l, m, c, e, d); //T11
	printf("%0.i, %0.d, %0.d, %0.d, %0.d, %0.d, %0.d, %0.d\n", i, j, k, l, m, c, e, d); //T12
	ft_printf("%0.i, %0.d, %0.d, %0.d, %0.d, %0.d, %0.d, %0.d\n", i, j, k, l, m, c, e, d); //T12
	printf("%.0i, %.0d, %.0d, %.0d, %.0d, %.0d, %.0d, %.0d\n", i, j, k, l, m, c, e, d); //T13
	ft_printf("%.0i, %.0d, %.0d, %.0d, %.0d, %.0d, %.0d, %.0d\n", i, j, k, l, m, c, e, d); //T13
	printf("%0.0i, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d\n", i, j, k, l, m, c, e, d); //T14
	ft_printf("%0.0i, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d, %0.0d\n", i, j, k, l, m, c, e, d); //T14
	printf("%-0i, %-0d, %-0d, %-0d, %-0d, %-0d, %-0d, %-0d\n", i, j, k, l, m, c, e, d); //T15
	ft_printf("%-0i, %-0d, %-0d, %-0d, %-0d, %-0d, %-0d, %-0d\n", i, j, k, l, m, c, e, d); //T15
	printf("%0-i, %0-d, %0-d, %0-d, %0-d, %0-d, %0-d, %0-d\n", i, j, k, l, m, c, e, d); //T16
	ft_printf("%0-i, %0-d, %0-d, %0-d, %0-d, %0-d, %0-d, %0-d\n", i, j, k, l, m, c, e, d); //T16
	printf("%-.i, %-.d, %-.d, %-.d, %-.d, %-.d, %-.d, %-.d\n", i, j, k, l, m, c, e, d); //T17
	ft_printf("%-.i, %-.d, %-.d, %-.d, %-.d, %-.d, %-.d, %-.d\n", i, j, k, l, m, c, e, d); //T17
	printf("%-.0i, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d\n", i, j, k, l, m, c, e, d); //T18
	ft_printf("%-.0i, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d, %-.0d\n", i, j, k, l, m, c, e, d); //T18
	printf("%-i, %-d, %-d, %-d, %-d, %-d, %-d, %-d\n", i, j, k, l, m, c, e, d); //T19
	ft_printf("%-i, %-d, %-d, %-d, %-d, %-d, %-d, %-d\n", i, j, k, l, m, c, e, d); //T19
	printf("%.i, %.d, %.d, %.d, %.d, %.d, %.d, %.d\n", i, j, k, l, m, c, e, d); //T20
	ft_printf("%.i, %.d, %.d, %.d, %.d, %.d, %.d, %.d\n", i, j, k, l, m, c, e, d); //T20
	printf("%00i, %00d, %00d, %00d, %00d, %00d, %00d, %00d\n", i, j, k, l, m, c, e, d); //T22
	ft_printf("%00i, %00d, %00d, %00d, %00d, %00d, %00d, %00d\n", i, j, k, l, m, c, e, d); //T22
	printf("%1.i, %1.d, %1.d, %1.d, %1.d, %1.d, %1.d, %1.d\n", i, j, k, l, m, c, e, d); //T23
	ft_printf("%1.i, %1.d, %1.d, %1.d, %1.d, %1.d, %1.d, %1.d\n", i, j, k, l, m, c, e, d); //T23
	printf("%.1i, %.1d, %.1d, %.1d, %.1d, %.1d, %.1d, %.1d\n", i, j, k, l, m, c, e, d); //T24
	ft_printf("%.1i, %.1d, %.1d, %.1d, %.1d, %.1d, %.1d, %.1d\n", i, j, k, l, m, c, e, d); //T24
	printf("%1.1i, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d\n", i, j, k, l, m, c, e, d); //T25
	ft_printf("%1.1i, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d, %1.1d\n", i, j, k, l, m, c, e, d); //T25
	printf("%-1i, %-1d, %-1d, %-1d, %-1d, %-1d, %-1d, %-1d\n", i, j, k, l, m, c, e, d); //T26
	ft_printf("%-1i, %-1d, %-1d, %-1d, %-1d, %-1d, %-1d, %-1d\n", i, j, k, l, m, c, e, d); //T26
	printf("%-01i, %-01d, %-01d, %-01d, %-01d, %-01d, %-01d, %-01d\n", i, j, k, l, m, c, e, d); //T27
	ft_printf("%-01i, %-01d, %-01d, %-01d, %-01d, %-01d, %-01d, %-01d\n", i, j, k, l, m, c, e, d); //T27
	printf("%-1.i, %-1.d, %-1.d, %-1.d, %-1.d, %-1.d, %-1d, %-1d\n", i, j, k, l, m, c, e, d); //T28
	ft_printf("%-1.i, %-1.d, %-1.d, %-1.d, %-1.d, %-1.d, %-1d, %-1d\n", i, j, k, l, m, c, e, d); //T28
	printf("%-.1i, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d\n", i, j, k, l, m, c, e, d); //T29
	ft_printf("%-.1i, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d, %-.1d\n", i, j, k, l, m, c, e, d); //T29
	printf("%-0.1i, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d\n", i, j, k, l, m, c, e, d); //T30
	ft_printf("%-0.1i, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d, %-0.1d\n", i, j, k, l, m, c, e, d); //T30
	printf("%-1.0i, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d\n", i, j, k, l, m, c, e, d); //T31
	ft_printf("%-1.0i, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d, %-1.0d\n", i, j, k, l, m, c, e, d); //T31
	printf("%-1.1i, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d\n", i, j, k, l, m, c, e, d); //T32
	ft_printf("%-1.1i, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d, %-1.1d\n", i, j, k, l, m, c, e, d); //T32
	printf("%1.0i, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d\n", i, j, k, l, m, c, e, d); //T33
	ft_printf("%1.0i, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d, %1.0d\n", i, j, k, l, m, c, e, d); //T33
	while (a < 5) //T34-69
	{
		printf("%*i, %*d, %*d, %*d, %*d, %*d, %*d, %*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		ft_printf("%*i, %*d, %*d, %*d, %*d, %*d, %*d, %*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		printf("%0*i, %0*d, %0*d, %0*d, %0*d, %0*d, %0*d, %0*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		ft_printf("%0*i, %0*d, %0*d, %0*d, %0*d, %0*d, %0*d, %0*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		printf("%-*i, %-*d, %-*d, %-*d, %-*d, %-*d, %-*d, %-*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		ft_printf("%-*i, %-*d, %-*d, %-*d, %-*d, %-*d, %-*d, %-*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		printf("%.*i, %.*d, %.*d, %.*d, %.*d, %.*d, %.*d, %.*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		ft_printf("%.*i, %.*d, %.*d, %.*d, %.*d, %.*d, %.*d, %.*d\n", a, i, a, j, a, k, a, l, a, m, a, c, a, e, a, d);
		a++;
	}
	a = -2;
	while(a < 5) //T70-177
	{
		b = -2;
		while (b < 5)
		{
			printf("%*.*i, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			ft_printf("%*.*i, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d, %*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			printf("%-*.*i, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			ft_printf("%-*.*i, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d, %-*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			printf("%0*.*i, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			ft_printf("%0*.*i, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d, %0*.*d\n", a, b, i, a, b, j, a, b, k, a, b, l, a, b, m, a, b, c, a, b, e, a, b, d);
			b++;
		}
		a++;
	}
	return (0);
}
