/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:23 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/16 20:59:56 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
#define FT_PING_H

// STANDARD LIBS FOR PARSING AND PRINTING
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

// LIBS FOR SOCKETS AND NETWORK
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/time.h> 	// hints
#include <netdb.h>		// timeval

// FOR CTRL-C
#include <signal.h>

#include "errors.h"

// BITWISE FLAGS FOR OPTIONS
# define OPT_COUNT		0b1
# define OPT_TTL		0b10
# define OPT_VERBOSE	0b100
# define OPT_LINGER		0b1000
# define OPT_INTERVAL	0b10000
# define OPT_QUIET		0b100000

# define MAX_DEST		10
# define MAX_DEST_SIZE	256
# define PACK_SIZE		64


typedef struct {
	uint16_t	flags;
	uint16_t	nb_pkt;
	uint16_t	ttl;
	uint16_t	linger_time;
	uint16_t	interval;
	uint16_t	nb_dests;
	char		dests[MAX_DEST][MAX_DEST_SIZE];
} t_parser;

typedef struct {
	int					sockfd;
	struct addrinfo		*resolved;
	struct addrinfo		hints;

	struct sockaddr_in	addr;
	socklen_t			addrlen;
	
	uint16_t			id;
	uint16_t			seq;
	struct timeval		start_time;
} t_params;

typedef struct {
	struct icmphdr	header;
	char			content[PACK_SIZE - sizeof(struct icmphdr)];
} t_packet;

typedef struct {
	int		sent;
	int		received;

	double	min_time;
	double	max_time;
	double	total_time;
	double	sum_sq_time; // for stddev

	struct timeval begin; // For duration of the session
} t_stats;

void	parse_args(int argc, char *argv[], t_parser *cli_args);

void	ping_loop(t_parser *cli_args, t_params *params, t_packet *pkt, t_stats *stats);

uint16_t checksum(void *ptr, int len);

void	print_options(t_parser *options); //debug
void	print_help();

void	oops_crash(char* msg, char* try_help);

#endif
