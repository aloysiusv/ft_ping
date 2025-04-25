/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:23 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/25 19:35:59 by lrandria         ###   ########.fr       */
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

# define ICMP_HDR_SIZE	8
# define IP_HDR_SIZE	20
# define PAYLOAD_SIZE	56
# define PACKET_SIZE	PAYLOAD_SIZE + ICMP_HDR_SIZE
# define RESPONSE_SIZE	PACKET_SIZE + IP_HDR_SIZE


typedef struct {
	int					flags; //for options
	int					packet_count; // number of packets to send
	int					ttl; // timet-to-live
	int					timeout; //timeout option
	int					interval; // interval between pings
	int					dest_count; //number of dests
	char				*dest;
} t_parser;

typedef struct {
	// ICMP Header
	uint8_t  			type;
    uint8_t  			code;
    uint16_t 			id;
    uint16_t 			seq;
    uint16_t 			checksum;

	// Payload
	char 				payload[PAYLOAD_SIZE];
} t_packet;

typedef struct {
	t_packet			packet;
	
	char				*ip_dest;
	
	int					packets_sent;
    int					packets_received;
    int					packets_lost;

    double				rtt_min;
    double				rtt_max;
    double				rtt_sum;
    double				rtt_sum_sqr;   
} t_ping;

// in the loop: sockfd, sent time, rcv time

void	parse_args(int ac, char *av[], t_parser *args);

void	start_ping(t_parser *args, t_ping *ping);
int 	play_ping_pong(t_parser *args, t_ping *ping, int sockfd, struct addrinfo *resolved);

uint16_t checksum(void *ptr, int len);

void	print_options(t_parser options); //debug
void	print_start_infos( t_parser *args, t_ping *ping);
void	print_help();

void	oops_crash(char* msg, char* try_help);

#endif
