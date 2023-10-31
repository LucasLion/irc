#include "../includes/Server.hpp"

#include <iostream>
#include <string>
#include <vector>

void Server::splitMode(const std::string& modeArg, std::vector<std::string>& modeChanges) {
    std::string currentChange;
    bool isAdding = true;  
    for (std::string::const_iterator it = modeArg.begin(); it != modeArg.end(); ++it) {
        char c = *it;
        if (c == '+' || c == '-') {
            if (!currentChange.empty()) {
                modeChanges.push_back(currentChange);
            }
            currentChange = c;
            isAdding = (c == '+');
        } else {
            currentChange += c;
        }
    }
    if (!currentChange.empty()) {
        modeChanges.push_back(currentChange);
    }
}

void Server::parseMode(Channel* channel, User* user, const std::string& target, const std::string* modeArgs, int nbArgs) {
    std::string nick = user->getNickName();
    int sd = user->getSd();
	(void) sd;
	(void) nick;
	(void) target;
	(void) channel;
	std::cout << "parseMode" << std::endl;
	std::cout << "nbArgs : " << nbArgs << std::endl;

    // Iterate through the modeArgs to handle different modes
    for (int i = 0; i < nbArgs; i++) {
        std::string modeArg = modeArgs[i];
		std::cout << "modeArg : " << modeArg << std::endl;
        std::vector<std::string> modeChanges;
        //splitMode(modeArg, modeChanges);

        for (int j = 0; j < (int)modeArg.size(); j++) {
            char modeLetter = modeArg[j];
			bool isAdding;
			if (modeLetter == '+' ) 
			{
				isAdding = true;
				j++;
				if (j < (int)modeArg.size())
					modeLetter = modeArg[j];
			}
			if (modeLetter == '-') 
			{
				isAdding = false;
				j++;
				if (j < (int)modeArg.size())
					modeLetter= modeArg[j];
			}
			std::cout << "modeLetter : " << modeLetter << std::endl;
			std::cout << "isAdding : " << isAdding << std::endl;

            switch (modeLetter) {
                case 'i':
                     if (isAdding && !channel->isInviteOnly) {
                        std::cout << "on ajoute le mode i" << std::endl;
						channel->isInviteOnly = true;
                        sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+" ,"i"));
                    } else if (!isAdding && channel->isInviteOnly) {
                        channel->isInviteOnly= false;
						std::cout << "on enleve le mode i" << std::endl;
                        sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "i"));
                    }
                    break;
                case 't':
                    if (isAdding && !channel->isTopicProtected) {
						std::cout << "on ajoute le mode t" << std::endl;
                        channel->isTopicProtected = true;
                        sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "t"));
                    } else if (!isAdding && channel->isTopicProtected) {
						std::cout << "on enleve le mode t" << std::endl;
                        channel->isTopicProtected = false;
                        sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "t"));
                    }
                    break;
        //         case 'l':
        //              if (modeChange.size() > 1) {
        //                 if (isAdding) {
        //                     //changer le stoi
        //                     int userLimit = std::stoi(modeChange.substr(1));
        //                     channel->userLimit = userLimit;
        //                     channel->hasUserLimit = true;
        //                     Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "l"));
        //                 } else {
        //                     channel->hasUserLimit = false;
        //                     Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "l"));
        //                 }
        //             }
        //             break;
        //         case 'o':
        //             if (modeChange.size() > 1) {
        //                 if (isAdding) {
        //                     std::string operatorNick = modeChange.substr(1);
        //                     if (channel->isUserInChannel(operatorNick)) {
        //                         channel->addOperator(operatorNick);
        //                         Server::sendClient(sd, RPL_CHANNELMODEIS(nick, "+", "o"));
        //                     } else {
        //                         Server::sendClient(sd, ERR_NOTONCHANNEL(nick, target));
        //                     }
        //                 } else {
        //                     std::string operatorNick = modeChange.substr(1);
        //                     if (channel->isOperator(operatorNick)) {
        //                         channel->removeOperator(operatorNick);
        //                         Server::sendClient(sd, RPL_CHANNELMODEIS(nick, "-", "o"));
        //                     }
        //                 }
        //             }
        //             break;
        //         case 'k':
        //             if (modeChange.size() > 1) {
        //                 if (isAdding) {
        //                     std::string pass = modeChange.substr(1);
        //                     channel->password = pass;
        //                     Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "k"));
        //                 } else {
        //                     if (channel->isPasswordSet()) {
        //                         channel->removePassword();
        //                         Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "l"));
        //                     }
        //                }
        //             break;
                default:
					std::cout << "on envoie le mode inconnu" << std::endl;
                    Server::sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
                    break;
            }
        }
    }
}



void	Server::modeCmd( Message msg, User *user ) {
	
	std::cout << "modeCmd : " << msg.rawMessage	<< std::endl;
	Channel *channel;
	std::map<std::string, Channel*>::iterator it;
	std::string nick = user->getNickName();
	std::string target = msg.getParam(0);
	int sd = user->getSd();
	int nbArgs = msg.nbParam() - 1;
	std::string modeArgs[nbArgs];
	for(int i = 0; i < nbArgs; i++)
		modeArgs[i] = msg.getParam(1 + i);
	
	if(target[0] != '#')
	 	sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
	else {
		if(_channels.find(target) == _channels.end()){
			sendClient(sd, ERR_NOSUCHCHANNEL(nick, target));
			return;
		}
        //find the channel
		for (it = getChannels()->begin(); it != getChannels()->end(); ++it) {
				if (it->first == target)
					channel = it->second;
				std::cout << "Channel: " << it->first << std::endl;
		}
		// retouver les modes du channels
		if (nbArgs == 0) {
	  		sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", channel->getCurrentModes() ));
			return;
		}
		//check if the user is in the channel && is op
		if (channel->isUserInChannel(nick) == false) {
			sendClient(sd, ERR_NOTONCHANNEL(nick, target));
			return;
		}
		if (channel->isUserOp(nick) == false) {
			sendClient(sd, ERR_CHANOPRIVSNEEDED(nick, target));
			return;
		}
		 else  
		 	parseMode(channel, user, target, modeArgs, nbArgs);
	}
}
