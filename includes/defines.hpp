
#ifndef INC_42FT_IRC_ANSWERS_HPP
# define INC_42FT_IRC_ANSWERS_HPP

# define CHANNELLEN "512"
# define HOSTLEN "512"
# define KICKLEN "307"
# define NICKLEN "31"
# define TOPICLEN "307"
# define USERLEN "18"

# define RPL_WELCOME(client)				(":localhost 001 " + client + " :Welcome to FT_IRC Network, " + client + "\r\n")
# define RPL_YOURHOST(client)			(":localhost 002 " + client + " :Your host is localhost, running version 1\r\n")
# define RPL_CREATED(client, datetime)	(":localhost 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client)				(":localhost 004 " + client + " FT_IRC 1 i itkol\r\n")
# define RPL_ISUPPORT(client)			(":localhost 005 " + client +	" PREFIX=(qo)~@ " + \
																			"CASEMAPPING=rfc7613 " + \
																			"CHANLIMIT=#: " + \
																			"CHANMODES=,,kl,it " + \
																			"CHANNELLEN=" + CHANNELLEN + " " + \
																			"CHANTYPES=# " + \
																			"ELIST= " + \
																			"HOSTLEN=" + HOSTLEN + " " + \
																			"KICKLEN=" + KICKLEN + " " + \
																			"MAXTARGETS= " + \
																			"NETWORK=FT_IRC " + \
																			"NICKLEN=" + NICKLEN + " " + \
																			"STATUSMSG=~@ " + \
																			":Are supported by this server\r\n")
# define RPL_ISUPPORT2(client)			(":localhost 005 " + client +	" TARGMAX=PRIVMSG:,JOIN:,NAMES:,WHO:1,PART:,KICK:1,LIST:1 " + \
																			"TOPICLEN=" + TOPICLEN + " " + \
																			"USERLEN=" + USERLEN + " " + \
																			":Are supported by this server\r\n")

# define RPL_UMODEIS(client, modestring)	(":localhost 221 " + client + " " + modestring + "\r\n")
# define RPL_LUSERCLIENT(client, u, i, s)(":localhost 251 " + client + " :There are " + u + " users and " + i + " invisible on " + s + " servers\r\n")
# define RPL_LUSEROP(client, ops)		(":localhost 252 " + client + " " + ops + " :operator(s) online\r\n")
# define RPL_LUSERCHANNELS(client, ch)	(":localhost 254 " + client + " " + ch + " :channels formed\r\n")
# define RPL_LUSERME(client, c, s)		(":localhost 255 " + client + " :I have " + c + " clients and " + s + " servers\r\n")
# define RPL_LOCALUSERS(client, u, m)	(":localhost 265 " + client + " " + u + " " + m + ":Current local users " \
											+ u + ", max " + m + "\r\n")
# define RPL_GLOBALUSERS(client, u, m)	(":localhost 266 " + client + " " + u + " " + m + ":Current global users " \
											+ u + ", max " + m + "\r\n")

# define RPL_ENDOFWHO(client, mask)		(":localhost 315 " + client + " " + mask + " :End of WHO list\r\n")
# define RPL_LISTSTART(client)			(":localhost 321 " + client + " Channel : Users Name\r\n")
# define RPL_LIST(client, chan, count, \
								topic)	(":localhost 322 " + client + " " + chan + " " + count + " :" + topic + "\r\n")
# define RPL_LISTEND(client)				(":localhost 323 " + client + " :End of /LIST\r\n")
# define RPL_CHANNELMODEIS(client, \
	chan, modestring, modearguments)	(":localhost 324 " + client + " " + chan + " " + modestring + " " + modearguments + "\r\n")
# define RPL_CREATIONTIME(client, chan, \
								time)	(":localhost 329 " + client + " " + chan + " " + time + " \r\n")
# define RPL_NOTOPIC(client, chan)		(":localhost 331 " + client + " " + chan + " :No topic is set\r\n")
# define RPL_TOPIC(client, chan, topic)	(":localhost 332 " + client + " " + chan + " :" + topic + "\r\n")
# define RPL_TOPICWHOTIME(client, chan, \
							setter, at)	(":localhost 333 " + client + " " + chan + " " + setter + " " + at + "\r\n")
# define RPL_INVITING(client, nick,chan)	(":localhost 341 " + client + " " + nick + " " + chan + "\r\n")
# define RPL_WHOREPLY(client, chan, \
				user, nick, realname)	(":localhost 352 " + client + " " + chan + " " + user + " " + g_host + " " \
										+ " FT_IRC " + nick + " H :0 " + realname + "\r\n")
# define RPL_NAMREPLY(client, symbol, \
					chan, prefix, nick)	(":localhost 353 " + client + " " + symbol + " " + chan + " :" + prefix + nick + "\r\n")
# define RPL_ENDOFNAMES(client, chan)	(":localhost 366 " + client + " " + chan + " :End of /NAMES list\r\n")
# define RPL_MOTDSTART(client)			(":localhost 375 " + client + " :- FT_IRC Message of the day - \r\n")
# define RPL_MOTD(client, line)			(":localhost 372 " + client + " :" + line + "\r\n")
# define RPL_MOTDEND(client)				(":localhost 376 " + client + " :End of /MOTD command.\r\n")

# define ERR_NOSUCHNICK(client, target)	(":localhost 401 " + client + " " + target + " :No such nickname/channel\r\n")
# define ERR_NOSUCHCHANNEL(client, chan)	(":localhost 403 " + client + " " + chan + " :No such channel\r\n")
# define ERR_NORECIPIENT(client)			(":localhost 411 " + client + " :No recipient given\r\n")
# define ERR_NOTEXTTOSEND(client)		(":localhost 412 " + client + " :No text to send\r\n")
# define ERR_INPUTTOOLONG(client)		(":localhost 417 " + client + " :Input line was too long\r\n")
# define ERR_UNKNOWNCOMMAND(client, com)	(":localhost 421 " + client + " " + com + " :No such command\r\n")
# define ERR_NONICKNAMEGIVEN(client)		(":localhost 431 " + client + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(client, \
								nick)	(":localhost 432 " + client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick)	(":localhost 433 " + client + " " + nick + " :Nickname is already in use\r\n")
# define ERR_USERNOTINCHANNEL(client, \
							nick, chan)	(":localhost 441 " + client + " " + nick + " " + chan + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(client, chan)	(":localhost 442 " + client + " " + chan + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(client, \
							nick, chan)	(":localhost 443 " + client + " " + nick + " " + chan + " :Is already on channel\r\n")
# define ERR_BADCHANNAME(client, chan)	(":localhost 448 " + client + " " + chan + " :Channel name must start with a hash mark (#)\r\n")
# define ERR_NOTREGISTERED(client)		(":localhost 451 " + client + " :You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(client, com)	(":localhost 461 " + client + " " + com + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(client)	(":localhost 462 " + client + " :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH(client)		(":localhost 464 " + client + " :Password incorrect\r\n")
# define ERR_CHANNELISFULL(client, chan)	(":localhost 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n")
# define ERR_UNKNOWNMODE(client, chara)	(":localhost 472 " + client + " " + chara + " :Is unknown mode char to me\r\n")
# define ERR_MODESTRINGERR(client, \
							modestring)	(":localhost 472 " + client + " " + modestring + " :Modestring starts with a + or a -\r\n")
# define ERR_INVITEONLYCHAN(client, chan)(":localhost 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n")
# define ERR_BADCHANNELKEY(client, chan)	(":localhost 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n")
# define ERR_CHANOPRIVSNEEDED(client, \
								chan)	(":localhost 482 " + client + " " + chan + " :You're not a channel operator\r\n")

# define ERR_UMODEUNKNOWNFLAG(client)	(":localhost 501 " + client + " :Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH(client)		(":localhost 502 " + client + " :Cant change mode for other user\r\n")

# define	ERR_ISFOUNDER(client, nick)		(":localhost 701 " + client + " " + nick + " :Couldn't perform action because user is founder\r\n")
# define ERR_TOOLONG(client, size, limit)(":localhost 702 " + client + " " + size + " :size too long (limit is " + limit + ")\r\n")

# define NICK(sender, newn)				(":" + sender + " NICK " + newn + "\r\n")
# define PRIVMSG(sender, receiver, msg)	(":" + sender + " PRIVMSG " + receiver + " :" + msg + "\r\n")
# define JOIN(sender, target)			(":" + sender + " JOIN :" + target + "\r\n")
# define INVITE(sender, target, chan)	(":" + sender + " INVITE " + target + " " + chan + "\r\n")
# define TOPIC(sender, chan, topic)		(":" + sender + " TOPIC " + chan + " :" + topic + "\r\n")
# define MODE(sender, target, mode, arg)	(":" + sender + " MODE " + target + " " + mode + " " + arg + "\r\n")
# define PART(sender, chan, reason)		(":" + sender + " PART " + chan + " :" + reason + "\r\n")
# define KICK(sender, chan, nick, reason)(":" + sender + " KICK " + chan + " " + nick + " :" + reason + "\r\n")
# define QUIT(sender, reason)			(":" + sender + " QUIT :" + reason + "\r\n")
# define PONG(token)						(":localhost PONG localhost " + token + "\r\n")

#endif



// NICK amouly
// USER useramouly 0 * :realamouly
// NICK moulnick