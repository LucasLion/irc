/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llion@student.42mulhouse.fr <marvin@42.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:29:58 by llion@student     #+#    #+#             */
/*   Updated: 2023/10/06 14:11:04 by llion@student    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Parser.hpp"

int main(int argc , char *argv[])
{
	static_cast<void>(argc);
	Server	server;
	
	server.setPort(argv[1]);
	server.loop(argc, argv);
	
	
	return 0;
}

