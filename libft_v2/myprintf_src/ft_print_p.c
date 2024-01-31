/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_p.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skanna <skanna@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 11:16:05 by skanna            #+#    #+#             */
/*   Updated: 2023/11/07 11:21:10 by skanna           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	ft_print_p2(unsigned long long n)
{
	char	*base;
	int		len;

	base = "0123456789abcdef";
	len = 0;
	if (n >= 16)
		len += ft_print_p2(n / 16);
	len += (ft_putchar(base[n % 16]));
	return (len);
}

int	ft_print_p1(unsigned long long n)
{
	char	*hex;
	int		len;

	len = 0;
	hex = "0x";
	len += ft_putstr(hex);
	len += ft_print_p2(n);
	return (len);
}
