/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:23 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/01 22:03:02 by lrandria         ###   ########.fr       */
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

// FOR SELECT() AND ERRNO
#include <errno.h>
#include <sys/select.h>

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
	char 				buffer[RESPONSE_SIZE];
	struct iphdr 		*ip_hdr;
	int 				ip_hdr_len;
	struct icmphdr 		*icmp_hdr;
} t_response;

typedef struct {
	t_packet			packet;
	t_response			response;
	
	int					sockfd;
	char				*ip_dest;
	struct addrinfo		*resolved;
    
	// Stats
	int					packets_lost;
    double				rtt_min;
    double				rtt_max;
    double				rtt_sum;
    double				rtt_sum_sqr;
} t_ping;

void	parse_args(int ac, char *av[], t_parser *args);

void	start_ping(t_parser *args, t_ping *ping);
int 	play_ping_pong(t_parser *args, t_ping *ping);

uint16_t checksum(void *ptr, int len);

void	print_start_infos(const t_parser *args, const t_ping *ping);
void	print_current_infos(const t_ping *ping, const t_response *rsp, const int bytes, double rtt);
void 	print_end_infos(const t_ping *ping);
void	print_errors(t_ping *ping, const int bytes, const int flags);
void	print_help();

void	oops_crash(const char* msg, const char* try_help);

#endif
