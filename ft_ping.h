/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:23 by lrandria          #+#    #+#             */
/*   Updated: 2025/05/05 11:17:08 by lrandria         ###   ########.fr       */
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
#include <netinet/ip_icmp.h> // hints
#include <sys/time.h> // timeval
#include <netdb.h>
#include <sys/select.h>

// FOR CTRL-C
#include <signal.h>

// FOR CALCULATING STATS
#include <math.h>

// ERRORS MSG
# define E_NOT_SUDO          "ft_ping: Are you sudo...?"
# define E_MISSING_DEST      "ft_ping: missing host operand"
# define E_BAD_DEST          "ft_ping: unknown host"
# define E_MAX_DEST          "ft_ping: only one destination needed"
# define E_MISSING_ARG       "ft_ping: option requires an argument"
# define E_BAD_VALUE         "ft_ping: invalid value: "
# define E_TOO_SMALL         "ft_ping: option value too small: "
# define E_TOO_BIG           "ft_ping: option value too big: "
# define E_WTF_OPT           "ft_ping: unrecognised option: "
# define E_SOCK_ERROR        "ft_ping: 'socket()' error"
# define E_SETSOCKOPT_ERROR  "ft_ping: 'setsockopt()' error"
# define E_INTERNAL_ERROR    "ft_ping: internal error"
# define E_TRY_HELP          "\nTry 'ft_ping -?', 'ft_ping --help' or 'ft_ping --usage' for more information"

// BITWISE FLAGS FOR OPTIONS
# define OPT_COUNT      0b1
# define OPT_TTL        0b10
# define OPT_VERBOSE    0b100
# define OPT_LINGER     0b1000
# define OPT_INTERVAL   0b10000
# define OPT_QUIET      0b100000

// FIXED SIZE
# define ICMP_HDR_SIZE  8
# define IP_HDR_SIZE    20
# define PAYLOAD_SIZE   56
# define PACKET_SIZE    PAYLOAD_SIZE + ICMP_HDR_SIZE
# define RESPONSE_SIZE  PACKET_SIZE + IP_HDR_SIZE

typedef struct {
    int                 flags; // For all options
    int                 packet_count; // Nb of packets to send
    int                 ttl; // Time-to-live
    int                 linger; // How long you wait for a resp
    int                 interval; // How long you wait before sending a packet
    char                *dest;
} t_parser;

typedef struct {
    // ICMP Header
    uint8_t  			type;
    uint8_t  			code;
    uint16_t 			checksum;
    uint16_t 			id;
    uint16_t 			seq;

    char                payload[PAYLOAD_SIZE];
} t_packet;

typedef struct {
    char                buffer[RESPONSE_SIZE];
    struct iphdr 		*ip_hdr;
    int                 ip_hdr_len;
    struct icmphdr 		*icmp_hdr;
} t_response;

typedef struct {
    t_packet			packet;
    t_response			response;
    
    int                 sockfd;
    char                *ip_dest;
    struct addrinfo		*resolved;
    
    // Stats to print
    int                 packets_lost;
    int                 packets_sent;
    double              rtt_min;
    double              rtt_max;
    double              rtt_sum;
    double              rtt_sum_sqr;
} t_ping;

void	parse_args(int ac, char *av[], t_parser *args);

void	start_ping(t_parser *args, t_ping *ping);
int 	play_ping_pong(t_parser *args, t_ping *ping);

void	print_start_infos(const t_parser *args, const t_ping *ping);
void	print_response_infos(const t_response *rsp, const int bytes, double rtt);
void 	print_end_infos(const t_ping *ping, char *dest);
void	print_errors(t_ping *ping, const int bytes, const int flags);
void	print_help();

void	oops_crash(const char* msg, const char* try_help);

#endif
