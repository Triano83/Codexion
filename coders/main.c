/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jotriano <jotriano@student.42malaga.com>   #+#  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026-06-25 08:19:44 by jotriano          #+#    #+#             */
/*   Updated: 2026-06-25 08:19:44 by jotriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int	is_numeric_str(char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	int	i;
	t_data data;
	if (argc != 9)
		return(printf("Error: Numero de argumentos invalidos\n"),1);
	i = 1;
	while (i++ <= 7)
	{
		if (!is_numeric_str(argv[i-1]))
			return(printf("Error: El argumento %d debe ser un entero positivo\n", i-1),1);
	}
	if (strcmp(argv[8], "fifo") != 0 && strcmp(argv[8], "edf") != 0)
		return(printf("Error: El planificador debe ser 'fifo' o 'edf'\n"),1);
	return (0);
	enter_data(&data, argv);
	if (!allocate_resources(&data))
		return (printf("Error de memoria en malloc\n"), 1);
	if (!init_dongles(&data) || !init_coders(&data))
		return (printf("Error inicializando los datos\n"), 1);
}
