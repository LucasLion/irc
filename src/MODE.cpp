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
			else{
                sendClient(sd, ERR_NEEDMOREPARAMS(nick, "MODE"));
                return;
            }
        }	
		std::cout << "modeLetter : " << modeLetter << std::endl;
		std::cout << "isAdding : " << isAdding << std::endl;

        switch (modeLetter) {
            case 'i':
                if (isAdding && !channel->isInviteOnly) {
					channel->isInviteOnly = true;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "+i", ""));
                } else if (!isAdding && channel->isInviteOnly) {
                    channel->isInviteOnly= false;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "-i", ""));
                }
                break;
            case 't':
                if (isAdding && !channel->isTopicProtected) {
                    channel->isTopicProtected = true;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "+t", ""));
                } else if (!isAdding && channel->isTopicProtected) {
                    channel->isTopicProtected = false;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "-t", ""));
                }
                break;
            case 'l':
                if (isAdding && modeArgs[i].size() > 0 && isdigit(modeArgs[i][0])) {
                        int userLimit = atoi(modeArgs[i].c_str());
                        i++;
                        channel->userLimit = userLimit;
                        channel->hasUserLimit = true;
                        channel->sendMessgeToAllUsers(MODE(nick, target, "+l", ""));
                    } else if (!isAdding && channel->hasUserLimit){
                        channel->hasUserLimit = false;
                        channel->sendMessgeToAllUsers(MODE(nick, target, "-l", ""));
                    } 
                break;
            case 'o':
                if (nbArgs != 0 && modeArgs[i].size() > 0) {
                    std::string operatorNick = modeArgs[i];
                    i++;
                    if (channel->isUserInChannel(operatorNick)) {
                        if (isAdding && !channel->isUserOp(operatorNick)) {
                            channel->addOperator(operatorNick);
                            Server::sendClient(channel->usersSd[operatorNick], RPL_CHANNELMODEIS(operatorNick, target, "+", "o"));
                            Server::sendClient(sd, RPL_CHANNELMODEIS(operatorNick, target, "+", "o"));
                        } else {
                            if (channel->isUserOp(operatorNick)) {
                                channel->removeOperator(operatorNick);
                                Server::sendClient(sd, RPL_CHANNELMODEIS(operatorNick, target, "-", "o"));
                                Server::sendClient(channel->usersSd[operatorNick], RPL_CHANNELMODEIS(operatorNick, target, "-", "o"));
                            }
                        }
                    } else {
                        Server::sendClient(sd,ERR_USERNOTINCHANNEL(nick, operatorNick, target));
                    }
                } else {
                        std::string opNick = channel->getChanNick(nick);
                        channel->sendMessgeToAllUsers(MODE(nick, target, "+o", ""));
                     //sendClient(sd, MODE(opNick, target, "+o", ""));
                }
                break;
            case 'k':
                if (isAdding && (modeArgs[i].size() > 0)) {       
                    std::string pass = modeArgs[i];
                    i++;
                    channel->password = pass;
                    channel->hasPassword = true;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "+k", ""));
                } else if (!isAdding && channel->hasPassword){
                    channel->hasPassword = false;
                    channel->sendMessgeToAllUsers(MODE(nick, target, "-k", ""));
                    }
                break;
            default:
                break;
            }
        }
    }



void	Server::modeCmd( Message msg, User *user ) {
	
    std::cout << "modeCmd : " << msg.rawMessage	<< std::endl;
	std::string nick = user->getNickName();
    std::cout << "nbParam : " << msg.nbParam() << std::endl;
    int nbArgs = msg.nbParam();
	std::string target = msg.getParam(0);
    int sd = user->getSd();
    if(target[0] != '#'){
	 	sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
        return;
    }
    if(_channels.find(target) == _channels.end()){
		sendClient(sd, ERR_NOSUCHCHANNEL(nick, target));
		return;
	}
    if (_channels[target]->isUserInChannel(nick) == false){
			sendClient(sd, ERR_NOTONCHANNEL(nick, target));
			return;
	}
	if (nbArgs == 1){
        std::string opNick = _channels[target]->getChanNick(nick);
		sendClient(sd, RPL_CHANNELMODEIS(opNick, target, "+", _channels[target]->getCurrentModes() ));
    }
    else{
        std::string modestring = msg.getParam(1);
        nbArgs = msg.nbParam() - 2;
	    std::string modeArgs[nbArgs];
	    for(int i = 0; i < nbArgs; i++)
		    modeArgs[i] = msg.getParam(2 + i);
	
		if (_channels[target]->isUserOp(nick) == false){
			sendClient(sd, ERR_CHANOPRIVSNEEDED(nick, target));
			return;
		}
		 else  
		 	parseMode(_channels[target], user, target,modestring, modeArgs, nbArgs);
	}
}

