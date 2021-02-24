/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matt <maquentr@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/01 12:08:04 by matt              #+#    #+#             */
/*   Updated: 2021/02/24 14:59:24 by matt             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct	s_args
{
	int c;
	int width;
	int has_width;
	int prec;
	int has_prec;
	int minus;
	int has_zero;
	int zero;
	int has_star_width;
	int star_width;
	int has_star_prec;
	int star_prec;
	int res;
}				t_args;


#endif
