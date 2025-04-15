/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/15 19:36:35 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void init(t_params *params, t_packet *pkt, t_stats *stats) {
	
	memset(pkt, 0, sizeof(t_packet));
	memset(params, 0, sizeof(t_params));
	memset(stats, 0, sizeof(t_stats));
}

int main(int argc, char *argv[]) {

	if (geteuid() != 0)
		oops_crash(E_NOTSUDO, NULL);
	if (argc < 2)
		oops_crash(E_MISSING_DEST, E_TRYHELP);
	
	// PARSING
	t_parser	cli_args;
	memset(&cli_args, 0, sizeof(t_parser));
	parse_args(argc, argv, &cli_args);
	
	// EXEC
	t_params	params;
	t_packet	pkt;
	t_stats		stats;
	
	init(&params, &pkt, &stats);
	print_options(&cli_args); // debug
	ping(&cli_args, &params, &pkt, &stats);
	return 0;
}
