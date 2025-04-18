/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:23 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/18 20:41:44 by lrandria         ###   ########.fr       */
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
# define OPT_TIMEOUT	0b1000
# define OPT_INTERVAL	0b10000
# define OPT_QUIET		0b100000

# define MAX_DEST		10

typedef struct {
	uint16_t	flags; //for options
	uint16_t	nb_pkt; // number of packets to send
	uint16_t	ttl; // timet-to-live
	uint16_t	timeout; //timeout option
	uint16_t	interval; // interval between pings
	uint16_t	nb_dests; //number of dests
	char		*dests[MAX_DEST];
} t_parser;

typedef struct {
	char *dest;           // host or IP
    struct sockaddr_in addr;  // resolved address
    int sockfd;
    int id;
    int seq;
    struct timeval send_time;
    struct timeval recv_time;
} t_ping;

void	parse_args(int argc, char *argv[], t_parser *cli_args);

void	ping_loop(t_parser *args);

uint16_t checksum(void *ptr, int len);

void	print_options(t_parser *options); //debug
void	print_help();

void	oops_crash(char* msg, char* try_help);

#endif
