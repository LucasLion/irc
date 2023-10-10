/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llion@student.42mulhouse.fr <marvin@42.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 10:44:00 by llion@student     #+#    #+#             */
/*   Updated: 2023/10/10 10:45:40 by llion@student    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "header.hpp"
#include "Command.hpp"

class Command {
    
	public :

		void                        parseInput( void );
		char*                       generateResponse();
		void                        printCommand();
		std::string                 raw_message;

	private : 

		std::string                 _tag;
		std::string                 _source;
		std::string                 _command;
		std::vector<std::string>    _param; 

};
