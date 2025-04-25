/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:50 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/25 19:34:12 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

// void print_options(t_parser *options) { //debug
	
// 	printf("Options:\n");
// 	if (options->flags & OPT_COUNT) {
// 		printf("Count: %d\n", options->packet_count);
// 	}
// 	if (options->flags & OPT_TTL) {
// 		printf("TTL: %d\n", options->ttl);
// 	}
// 	if (options->flags & OPT_VERBOSE) {
// 		printf("Verbose option is set.\n");
// 	}
// 	if (options->flags & OPT_TIMEOUT) {
// 		printf("Timeout: %d\n", options->timeout);
// 	}
// 	if (options->flags & OPT_INTERVAL) {
// 		printf("Interval: %d\n", options->interval);
// 	}
// 	if (options->flags & OPT_QUIET) {
// 		printf("Quiet option is set.\n");
// 	}
// 	printf("Destinations:\n");
// 	for (int i = 0; i < options->dest_count; ++i)
// 		printf("=> %s\n", options->dest);
// }

void print_help() {
   
	fprintf(stdout, "Usage: ft_ping [OPTION...] HOST ...\n"
                    "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
                    "Options:\n\n"
                    "   -c,                        (count) stop after sending X packets\n"
                    "   -i,                        (interval) wait X seconds between sending each packet\n"
                    "   --ttl=N                    specify N as time-to-live\n"
                    "   -w,                        stop after X seconds\n"
                    "   -v,                        verbose output\n"
                    "   -q,                        quiet output\n"
                    "   -?, --help OR --usage      give this help list\n");
}

void print_start_infos( t_parser *args, t_ping *ping) {

	if (args->flags & OPT_VERBOSE) {
		int packet_id = getpid() & 0xFFFF;
		printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", args->dest, ping->ip_dest, packet_id, packet_id);
	}
	else
		printf("PING %s (%s): 56 data bytes\n", args->dest, ping->ip_dest);
}

void print_end_infos() {
	
}