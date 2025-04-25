/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lrandria <lrandria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 17:43:55 by lrandria          #+#    #+#             */
/*   Updated: 2025/04/25 14:39:26 by lrandria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERROR_H

# define E_NOT_SUDO			"ft_ping: Are you sudo...?\n"
# define E_MISSING_DEST		"ft_ping: missing host operand\n"
# define E_BAD_DEST         "ft_ping: unknown host\n"
# define E_DEST_BIG			"ft_ping: destination string is too long\n"
# define E_MAX_DEST			"ft_ping: only one destination needed\n"
# define E_MISSING_ARG		"ft_ping: option requires an argument\n"
# define E_BAD_VALUE		"ft_ping: invalid value\n"
# define E_TOO_SMALL		"ft_ping: option value too small\n"
# define E_WTF_OPT			"ft_ping: unrecognised option\n"
# define E_INTERNAL_ERROR   "ft_ping: internal error\n"
# define E_TRY_HELP			"Try 'ft_ping -?', 'ft_ping --help' or 'ft_ping --usage' for more information\n"

#endif