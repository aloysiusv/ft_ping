/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2_ping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 18:58:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/18 20:44:14 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

extern volatile sig_atomic_t g_stop;

// static void create_pack(t_params *params, t_packet *pkt) {
	
//     pkt->header.type = ICMP_ECHO;
//     pkt->header.code = 0;
//     pkt->header.un.echo.id = getpid() & 0xFFFF;
//     pkt->header.un.echo.sequence = params->seq;
// 	pkt->header.checksum = 0; // reset for each address
//     pkt->header.checksum = checksum(&pkt->header, sizeof(t_packet));
// }

// static void create_sock(t_parser *args, t_params *params) {
	
// 	struct timeval timeout; // timeout in caseof socket error

// 	timeout.tv_sec = 1;
// 	timeout.tv_usec = 0;
	
// 	params->sockfd = socket(params->hints.ai_family,
// 							params->hints.ai_socktype,
// 							params->hints.ai_protocol);
// 	if (params->sockfd < 0) {
// 		freeaddrinfo(params->resolved);
// 		oops_crash(E_INTERNAL_ERROR, NULL);
// 	}

// 	setsockopt(params->sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
// 	setsockopt(params->sockfd, IPPROTO_IP, IP_TTL, &args->ttl, sizeof(args->ttl));
// }

// static void resolve_dest(char* dest, t_params *params) {
	
// 	memset(&params->hints, 0, sizeof(struct addrinfo));
// 	params->hints.ai_family = AF_INET;
// 	params->hints.ai_socktype = SOCK_RAW;
// 	params->hints.ai_protocol = IPPROTO_ICMP;

// 	if (getaddrinfo(dest, NULL, &params->hints, &params->resolved) != 0) {
// 		freeaddrinfo(params->resolved);
// 		oops_crash(E_BAD_DEST, NULL);
// 	}
// }

void ping_loop(t_parser *args) {
	
	for (int i = 0; i < args->nb_dests; i++) { \
		t_ping instance;
		memset(&instance, 0, sizeof(t_ping));

		instance.dest = args->dests[i];
		instance.id = getpid() & 0xFFFFl;
	}
	// 	resolve_dest(args->dests[i], params);
	// 	create_pack(params, pkt);
	// 	create_sock(args, params);
	// 	print_start_infos();
	// 	if (g_stop == 0) {
	// 		for (int j = 0; 1; ++j) { // infinite loop by default
	// 			play_ping_pong();
	// 			update_pack();
	// 			sleep(args->interval);
	// 			if (j == args->nb_pkt)
	// 				break;
	// 		}
	// 	}
	// 	else
	// 		print_end_infos(stats);
	// }
}