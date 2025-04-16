/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stopping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 18:30:37 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/16 18:30:57 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void oops_crash(char* msg, char* try_help) {
    
	fprintf(stderr, "%s", msg);
    if (try_help)
        fprintf(stderr, "%s", try_help);
    exit(EXIT_FAILURE);
}

