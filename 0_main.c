/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/16 20:51:36 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

volatile sig_atomic_t g_stop = 0;

static void sig_handler(int sig) {
    (void)sig;
    g_stop = 1;
}

static void init_structs(t_parser *cli_args, t_params *params, t_packet *pkt, t_stats *stats) {
	
	memset(&cli_args, 0, sizeof(t_parser));
	memset(pkt, 0, sizeof(t_packet));
	memset(params, 0, sizeof(t_params));
	memset(stats, 0, sizeof(t_stats));
	
	// DEFAULT VALUES
	cli_args->nb_pkt = -1; // necessary for the main loop
	cli_args->interval = 1;
	cli_args->ttl = 64;
}

int main(int argc, char *argv[]) {

	t_parser	cli_args;
	t_params	params;
	t_packet	pkt;
	t_stats		stats;
	
	signal(SIGINT, sig_handler);
	
	if (geteuid() != 0)
		oops_crash(E_NOT_SUDO, NULL);
	if (argc < 2)
		oops_crash(E_MISSING_DEST, E_TRY_HELP);

	init_structs(&cli_args, &params, &pkt, &stats);
	parse_args(argc, argv, &cli_args);
	print_options(&cli_args); // debug
	ping_loop(&cli_args, &params, &pkt, &stats);
	return 0;
}
