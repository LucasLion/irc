/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llion@student.42mulhouse.fr <marvin@42.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:29:58 by llion@student     #+#    #+#             */
/*   Updated: 2023/10/06 12:34:11 by llion@student    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parser.hpp"

int main(int argc , char *argv[])
{
	static_cast<void>(argc);
	Server	server;
	Parser	parser;
	
	server.createSocket(argc, argv);
	parser.cap("CAP");
	
	
	return 0;
}

