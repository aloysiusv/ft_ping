/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   0_main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:40:35 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/18 20:42:12 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

volatile sig_atomic_t g_stop = 0;

static void sig_handler(int sig) {
    (void)sig;
    g_stop = 1;
}

static void init_default(t_parser *args) {
	
	memset(args, 0, sizeof(t_parser));

	// DEFAULT VALUES
	args->nb_pkt = -1; // necessary for the main loop
	args->interval = 1;
	args->ttl = 64;
}

int main(int argc, char *argv[]) {

	t_parser	args;
	
	signal(SIGINT, sig_handler);
	
	if (geteuid() != 0)
		oops_crash(E_NOT_SUDO, NULL);
	if (argc < 2)
		oops_crash(E_MISSING_DEST, E_TRY_HELP);

	init_default(&args);
	parse_args(argc, argv, &args);
	//print_options(&args); // debug
	ping_loop(&args);
	return 0;
}
