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

void Server::parseMode(Channel* channel, User* user, const std::string& target, const std::string& modestring, const std::string* modeArgs, int nbArgs) {
    std::string nick = user->getNickName();
    int sd = user->getSd();
	std::cout << "parseMode" << std::endl;
	std::cout << "nbArgs : " << nbArgs << std::endl;
    int i = 0;
    bool isAdding = true;

    if ((modestring[0] != '+') && (modestring[0] != '-')){
        sendClient(sd, ERR_MODESTRINGERR(nick, modestring));
        return;
    }
    for (int j = 0; j < (int)modestring.size(); j++) {
        char modeLetter = modestring[j];
		
		if ((modeLetter == '+' ) || (modeLetter == '-')) {
			if(modeLetter != '+')
                isAdding = false;
            j++;
			if (j < (int)modestring.size())
					modeLetter = modestring[j];
			else
                sendClient(sd, ERR_NEEDMOREPARAMS(nick, "MODE"));
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
            case 'l':
                if (modeArgs[i].size() > 0) {
                    if (isAdding) {
                        int userLimit = atoi(modeArgs[i].c_str());
                        i++;
						std::cout << "on ajoute le mode l" << std::endl;
						std::cout << "userLimit : " << userLimit << std::endl;
                        channel->userLimit = userLimit;
                        channel->hasUserLimit = true;
                        Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "l"));
                    } else {
                        channel->hasUserLimit = false;
                        Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "l"));
                    }
                }  
                break;
                case 'o':
                    if (modeArgs[i].size() > 0) {
                        std::string operatorNick = modeArgs[i];
                        i++;
                        if (isAdding) {
                            if (channel->isUserInChannel(operatorNick)) {
                                channel->addOperator(operatorNick);
                                Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "o"));
                            } else {
                                Server::sendClient(sd, ERR_NOTONCHANNEL(nick, target));
                            }
                        } else {
                            if (channel->isUserOp(operatorNick)) {
                                channel->removeOperator(operatorNick);
                                Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "o"));
                            }
                        }
                    }
                    break;
                case 'k':
                    if (modeArgs[i].size() > 0) {
                        if (isAdding) {
                            std::string pass = modeArgs[i];
                            i++;
                            channel->password = pass;
                            channel->hasPassword = true;
                            Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", "k"));
                        } else {
                            if (channel->hasPassword) {
                                channel->hasPassword = false;
                                Server::sendClient(sd, RPL_CHANNELMODEIS(nick, target, "-", "l"));
                            }
                       }
                    break;
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
	std::string nick = user->getNickName();
	std::string target = msg.getParam(0);
    std::string modestring = msg.getParam(1);
	int sd = user->getSd();
	int nbArgs = msg.nbParam() - 2;
	std::string modeArgs[nbArgs];
	for(int i = 0; i < nbArgs; i++)
		modeArgs[i] = msg.getParam(2 + i);
	
	if(target[0] != '#')
	 	sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
	else {
		if(_channels.find(target) == _channels.end()){
			sendClient(sd, ERR_NOSUCHCHANNEL(nick, target));
			return;
		}
		if (nbArgs == 0){
	  		sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+", _channels[target]->getCurrentModes() ));
			return;
		}
		if (_channels[target]->isUserInChannel(nick) == false){
			sendClient(sd, ERR_NOTONCHANNEL(nick, target));
			return;
		}
		if (_channels[target]->isUserOp(nick) == false){
			sendClient(sd, ERR_CHANOPRIVSNEEDED(nick, target));
			return;
		}
		 else  
		 	parseMode(_channels[target], user, target,modestring, modeArgs, nbArgs);
	}
}
