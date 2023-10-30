#include "../includes/Server.hpp"

void splitMode(const std::string& modeArg, std::vector<std::string>& modeChanges) {
    std::string currentChange;
    bool isAdding = true;  // True for adding modes, false for removing
    for (char c : modeArg) {
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

void parseMode(Channel* channel, User* user, const std::string& target, const std::string* modeArgs, int nbArgs) {
    std::string nick = user->getNickName();
    int sd = user->getSd();

    // Iterate through the modeArgs to handle different modes
    for (int i = 0; i < nbArgs; i++) {
        std::string modeArg = modeArgs[i];
        std::vector<std::string> modeChanges;
        splitMode(modeArg, modeChanges);

        for (const std::string& modeChange : modeChanges) {
            char modeLetter = modeChange[0];
            bool isAdding = (modeChange[0] == '+');
            
            // Flag to indicate if the mode was handled
            bool modeHandled = true;

            switch (modeLetter) {
                case 'i':
                    // Handle Invite-Only (i) Channel Mode
                    // ... (as previously implemented)
                    break;
                case 't':
                    // Handle Protected Topic (t) Channel Mode
                    // ... (as previously implemented)
                    break;
                case 'l':
                    // Handle User Limit (l) Channel Mode
                    // ... (as previously implemented)
                    break;
                case 'o':
                    // Handle Operator (o) Channel Mode
                    // ... (as previously implemented)
                    break;
                case 'k':
                    // Handle Password (k) Channel Mode
                    // ... (as previously implemented)
                    break;
                default:
                    // Mode not supported, return ERR_UMODEUNKNOWNFLAG
                    modeHandled = false;
                    break;
            }

            if (modeHandled) {
                // Send appropriate responses if the mode was handled
            } else {
                // Mode not supported, return ERR_UMODEUNKNOWNFLAG
                sendClient(sd, ERR_UMODEUNKNOWNFLAG(nick));
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

		// retouver les modes du channels
		if (nbArgs == 0){
	  		sendClient(sd, RPL_CHANNELMODEIS(nick, target, "+,-", "i,t,k,o,l"));
			return;
		}
		//find the channel
		for (it = getChannels()->begin(); it != getChannels()->end(); ++it) {
				if (it->first == target)
					channel = it->second;
				std::cout << "Channel: " << it->first << std::endl;
			}
		//check if the user is in the channel && is op
		if (channel->isUserInChannel(nick) == false){
			sendClient(sd, ERR_NOTONCHANNEL(nick, target));
			return;
		}
		if (channel->isUserOp(nick) == false){
			sendClient(sd, ERR_CHANOPRIVSNEEDED(nick, target));
			return;
		}
		

	// // else  
	// // 	sendClient(user->getSd(), "OOOOOOKKKKKKKK");
	}
}