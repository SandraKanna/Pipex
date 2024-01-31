/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 14:56:32 by skanna            #+#    #+#             */
/*   Updated: 2023/11/08 15:33:41 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	print_type(va_list arg, char specifier)
{
	int	len;

	len = 0;
	if (specifier == 'c')
		len += ft_putchar(va_arg(arg, int));
	else if (specifier == 's')
		len += ft_putstr(va_arg(arg, char *));
	else if (specifier == 'u')
		len += ft_putunsigned(va_arg(arg, unsigned int), specifier);
	else if (specifier == 'd' || specifier == 'i')
		len += ft_putsigned(va_arg(arg, int), specifier);
	else if (specifier == 'x' || specifier == 'X')
		len += ft_putunsigned(va_arg(arg, unsigned int), specifier);
	else if (specifier == 'p')
		len += ft_print_p1(va_arg(arg, unsigned long long));
	else
		len += write(1, &specifier, 1);
	return (len);
}

int	ft_printf(const char *format, ...)
{
	int		len;
	int		i;
	va_list	arg;

	len = 0;
	i = 0;
	if (!format)
		return (-1);
	va_start(arg, format);
	while (format[i])
	{
		if (format[i] == '%')
		{
			len += print_type(arg, format[++i]);
			i++;
		}
		else
		{
			len += ft_putchar(format[i]);
			i++;
		}
	}
	va_end(arg);
	return (len);
}
