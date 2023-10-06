/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llion@student.42mulhouse.fr <marvin@42.fr  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 12:30:44 by llion@student     #+#    #+#             */
/*   Updated: 2023/10/06 12:34:49 by llion@student    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

std::string Parser::cap( std::string str ) {
	std::cout << "On parse à cet endroit" << std::endl;

	if (str == "CAP")
		std::cout << "Exemple: str = 'CAP'" << std::endl;
	return str;
}
