/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1_parser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 16:52:57 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/16 20:22:36 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ping.h"

static void add_dest(t_parser *cli_args, char* dest) {
	
	if (cli_args->nb_dests >= MAX_DEST)
		oops_crash(E_MAX_DEST, NULL);

    if (strlen(dest) >= MAX_DEST_SIZE)
		oops_crash(E_DEST_BIG, NULL);

    strncpy(cli_args->dests[cli_args->nb_dests], dest, MAX_DEST_SIZE - 1);
    cli_args->dests[cli_args->nb_dests][MAX_DEST_SIZE - 1] = '\0';
    cli_args->nb_dests++;
}

static int check_value(const char *str) {

	char *endptr;
	long val;

	val = strtol(str, &endptr, 10);
	if (endptr == str || *endptr != '\0' || val > __UINT16_MAX__ || val < 0)
        oops_crash(E_BAD_VALUE, NULL);
	return (__uint16_t)val;
}

static void validate_opt(t_parser *options, int flag, char *arg) {
	
	__uint16_t val;

	options->flags |= flag;
	if (arg) {
		val = check_value(arg);
		if (flag == OPT_COUNT)
			options->nb_pkt = val;
		else if (flag == OPT_LINGER)
			options->linger_time = val;
		else if (flag == OPT_TTL)
			options->ttl = val;
		else if (flag == OPT_INTERVAL)
			options->interval = val;
	}
}

void parse_args(int argc, char *argv[], t_parser *cli_args) {

	for (int i = 1; i < argc; ++i) {
		if (argv[i][0] == '-') {
			if (strcmp(argv[i], "-?") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "--usage") == 0) {
				print_help();
				exit(EXIT_SUCCESS);
			}
			else if (strcmp(argv[i], "-c") == 0) {
				if (argv[i + 1] == NULL)
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(cli_args, OPT_COUNT, argv[++i]);
			}				
			else if (strcmp(argv[i], "-W") == 0) {
				if (argv[i + 1] == NULL) {
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(cli_args, OPT_LINGER, argv[++i]); 
			}
			else if (strncmp(argv[i], "--ttl=", 6) == 0) {
				if (strcmp(argv[i], "--ttl=") == 0)
					oops_crash(E_TOO_SMALL, NULL);
				validate_opt(cli_args, OPT_TTL, argv[i] + 6);
			}
			else if (strcmp(argv[i], "-i") == 0) {
				if (argv[i + 1] == NULL)
					oops_crash(E_MISSING_ARG, E_TRY_HELP);
				validate_opt(cli_args, OPT_INTERVAL, argv[++i]);
			}
			else if (strcmp(argv[i], "-v") == 0)
				validate_opt(cli_args, OPT_VERBOSE, NULL);
			else if (strcmp(argv[i], "-q") == 0)
				validate_opt(cli_args, OPT_QUIET, NULL);
			else
				oops_crash(E_WTF_OPT, NULL);
			}
		}
		else
        	add_dest(cli_args, argv[i]);
	}
}