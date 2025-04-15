/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:50 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/15 19:46:14 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

void oops_crash(char* msg, char* try_help) {
    
	fprintf(stderr, "%s", msg);
    if (try_help)
        fprintf(stderr, "%s", try_help);
    exit(EXIT_FAILURE);
}

void print_options(t_parser *options) { //debug
	
	printf("Options:\n");
	if (options->flags & OPT_COUNT) {
		printf("Count: %d\n", options->nb_pkt);
	}
	if (options->flags & OPT_TTL) {
		printf("TTL: %d\n", options->ttl);
	}
	if (options->flags & OPT_VERBOSE) {
		printf("Verbose option is set.\n");
	}
	if (options->flags & OPT_LINGER) {
		printf("Linger time: %d\n", options->linger_time);
	}
	if (options->flags & OPT_INTERVAL) {
		printf("Interval: %d\n", options->interval);
	}
	if (options->flags & OPT_QUIET) {
		printf("Quiet option is set.\n");
	}
	printf("Destinations:\n");
	for (int i = 0; i < options->nb_dests; ++i)
		printf("=> %s\n", options->dests[i]);
}

void print_help() {
   
	fprintf(stdout, "Usage: ft_ping [OPTION...] HOST ...\n"
                    "Send ICMP ECHO_REQUEST packets to network hosts.\n\n"
                    "Options:\n\n"
                    "   -c,                        (count) stop after sending X packets\n"
                    "   -i,                        (interval) wait X seconds between sending each packet\n"
                    "   --ttl=N                    specify N as time-to-live\n"
                    "   -W,                        number of seconds to wait for response\n"
                    "   -v,                        verbose output\n"
                    "   -q,                        quiet output\n"
                    "   -?, --help OR --usage      give this help list\n");
}

