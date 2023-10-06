/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llion@student.42mulhouse.fr <marvin@42.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:05:19 by llion@student     #+#    #+#             */
/*   Updated: 2023/10/06 13:33:14 by llion@student    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdio>
#include <cerrno>

class Server {

public:

	int		createSocket( int argc, char** argv );
	void	loop( int agrc, char**argv );

private:
	
	const int	_maxClients = 30;
	int			_clientSockets[];	

};
