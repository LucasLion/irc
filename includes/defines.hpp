
#ifndef INC_42FT_IRC_ANSWERS_HPP
# define INC_42FT_IRC_ANSWERS_HPP

std::string	g_host = "localhost";

# define CHANNELLEN 512
# define HOSTLEN 512
# define KICKLEN 307
# define NICKLEN 31
# define TOPICLEN 307
# define USERLEN 18

# define RPL_WELCOME(client)				(":" + g_host + " 001 " + client + " :Welcome to FT_IRC, " + client + "\r\n")
# define RPL_YOURHOST(client)			(":" + g_host + " 002 " + client + " :Your host is " + g_host + ", running version 1\r\n")
# define RPL_CREATED(client, datetime)	(":" + g_host + " 003 " + client + " :This server was created " + datetime + "\r\n")
# define RPL_MYINFO(client)				(":" + g_host + " 004 " + client + " FT_IRC 1 i itkol\r\n")
# define RPL_ISUPPORT(client)			(":" + g_host + " 005 " + client +	" PREFIX=(qo)~@ " + \
																			"CASEMAPPING=rfc7613 " + \
																			"CHANLIMIT=#: " + \
																			"CHANMODES=,,kl,it " + \
																			"CHANNELLEN=" + std::to_string(CHANNELLEN) + " " + \
																			"CHANTYPES=# " + \
																			"ELIST= " + \
																			"HOSTLEN=" + std::to_string(HOSTLEN) + " " + \
																			"KICKLEN=" + std::to_string(KICKLEN) + " " + \
																			"MAXTARGETS= " + \
																			"NETWORK=FT_IRC " + \
																			"NICKLEN=" + std::to_string(NICKLEN) + " " + \
																			"STATUSMSG=~@ " + \
																			":Are supported by this server\r\n")
# define RPL_ISUPPORT2(client)			(":" + g_host + " 005 " + client +	" TARGMAX=PRIVMSG:,JOIN:,NAMES:,WHO:1,PART:,KICK:1,LIST:1 " + \
																			"TOPICLEN=" + std::to_string(TOPICLEN) + " " + \
																			"USERLEN=" + std::to_string(USERLEN) + " " + \
																			":Are supported by this server\r\n")

# define RPL_UMODEIS(client, modestring)	(":" + g_host + " 221 " + client + " " + modestring + "\r\n")
# define RPL_LUSERCLIENT(client, u, i, s)(":" + g_host + " 251 " + client + " :There are " + u + " users and " + i + " invisible on " + s + " servers\r\n")
# define RPL_LUSEROP(client, ops)		(":" + g_host + " 252 " + client + " " + ops + " :operator(s) online\r\n")
# define RPL_LUSERCHANNELS(client, ch)	(":" + g_host + " 254 " + client + " " + ch + " :channels formed\r\n")
# define RPL_LUSERME(client, c, s)		(":" + g_host + " 255 " + client + " :I have " + c + " clients and " + s + " servers\r\n")
# define RPL_LOCALUSERS(client, u, m)	(":" + g_host + " 265 " + client + " " + u + " " + m + ":Current local users " \
											+ u + ", max " + m + "\r\n")
# define RPL_GLOBALUSERS(client, u, m)	(":" + g_host + " 266 " + client + " " + u + " " + m + ":Current global users " \
											+ u + ", max " + m + "\r\n")

# define RPL_ENDOFWHO(client, mask)		(":" + g_host + " 315 " + client + " " + mask + " :End of WHO list\r\n")
# define RPL_LISTSTART(client)			(":" + g_host + " 321 " + client + " Channel : Users Name\r\n")
# define RPL_LIST(client, chan, count, \
								topic)	(":" + g_host + " 322 " + client + " " + chan + " " + count + " :" + topic + "\r\n")
# define RPL_LISTEND(client)				(":" + g_host + " 323 " + client + " :End of /LIST\r\n")
# define RPL_CHANNELMODEIS(client, \
	chan, modestring, modearguments)	(":" + g_host + " 324 " + client + " " + chan + " " + modestring + " " + modearguments + "\r\n")
# define RPL_CREATIONTIME(client, chan, \
								time)	(":" + g_host + " 329 " + client + " " + chan + " " + time + " \r\n")
# define RPL_NOTOPIC(client, chan)		(":" + g_host + " 331 " + client + " " + chan + " :No topic is set\r\n")
# define RPL_TOPIC(client, chan, topic)	(":" + g_host + " 332 " + client + " " + chan + " :" + topic + "\r\n")
# define RPL_TOPICWHOTIME(client, chan, \
							setter, at)	(":" + g_host + " 333 " + client + " " + chan + " " + setter + " " + at + "\r\n")
# define RPL_INVITING(client, nick,chan)	(":" + g_host + " 341 " + client + " " + nick + " " + chan + "\r\n")
# define RPL_WHOREPLY(client, chan, \
				user, nick, realname)	(":" + g_host + " 352 " + client + " " + chan + " " + user + " " + g_host + " " \
										+ " FT_IRC " + nick + " H :0 " + realname + "\r\n")
# define RPL_NAMREPLY(client, symbol, \
					chan, prefix, nick)	(":" + g_host + " 353 " + client + " " + symbol + " " + chan + " :" + prefix + nick + "\r\n")
# define RPL_ENDOFNAMES(client, chan)	(":" + g_host + " 366 " + client + " " + chan + " :End of /NAMES list\r\n")
# define RPL_MOTDSTART(client)			(":" + g_host + " 375 " + client + " :- FT_IRC Message of the day - \r\n")
# define RPL_MOTD(client, line)			(":" + g_host + " 372 " + client + " :" + line + "\r\n")
# define RPL_MOTDEND(client)				(":" + g_host + " 376 " + client + " :End of /MOTD command.\r\n")

# define ERR_NOSUCHNICK(client, target)	(":" + g_host + " 401 " + client + " " + target + " :No such nickname/channel\r\n")
# define ERR_NOSUCHCHANNEL(client, chan)	(":" + g_host + " 403 " + client + " " + chan + " :No such channel\r\n")
# define ERR_NORECIPIENT(client)			(":" + g_host + " 411 " + client + " :No recipient given\r\n")
# define ERR_NOTEXTTOSEND(client)		(":" + g_host + " 412 " + client + " :No text to send\r\n")
# define ERR_INPUTTOOLONG(client)		(":" + g_host + " 417 " + client + " :Input line was too long\r\n")
# define ERR_UNKNOWNCOMMAND(client, com)	(":" + g_host + " 421 " + client + " " + com + " :No such command\r\n")
# define ERR_NONICKNAMEGIVEN(client)		(":" + g_host + " 431 " + client + " :No nickname given\r\n")
# define ERR_ERRONEUSNICKNAME(client, \
								nick)	(":" + g_host + " 432 " + client + " " + nick + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(client, nick)	(":" + g_host + " 433 " + client + " " + nick + " :Nickname is already in use\r\n")
# define ERR_USERNOTINCHANNEL(client, \
							nick, chan)	(":" + g_host + " 441 " + client + " " + nick + " " + chan + " :They aren't on that channel\r\n")
# define ERR_NOTONCHANNEL(client, chan)	(":" + g_host + " 442 " + client + " " + chan + " :You're not on that channel\r\n")
# define ERR_USERONCHANNEL(client, \
							nick, chan)	(":" + g_host + " 443 " + client + " " + nick + " " + chan + " :Is already on channel\r\n")
# define ERR_BADCHANNAME(client, chan)	(":" + g_host + " 448 " + client + " " + chan + " :Channel name must start with a hash mark (#)\r\n")
# define ERR_NOTREGISTERED(client)		(":" + g_host + " 451 " + client + " :You have not registered\r\n")
# define ERR_NEEDMOREPARAMS(client, com)	(":" + g_host + " 461 " + client + " " + com + " :Not enough parameters\r\n")
# define ERR_ALREADYREGISTERED(client)	(":" + g_host + " 462 " + client + " :You may not reregister\r\n")
# define ERR_PASSWDMISMATCH(client)		(":" + g_host + " 464 " + client + " :Password incorrect\r\n")
# define ERR_CHANNELISFULL(client, chan)	(":" + g_host + " 471 " + client + " " + chan + " :Cannot join channel (+l)\r\n")
# define ERR_UNKNOWNMODE(client, chara)	(":" + g_host + " 472 " + client + " " + chara + " :Is unknown mode char to me\r\n")
# define ERR_MODESTRINGERR(client, \
							modestring)	(":" + g_host + " 472 " + client + " " + modestring + " :Modestring starts with a + or a -\r\n")
# define ERR_INVITEONLYCHAN(client, chan)(":" + g_host + " 473 " + client + " " + chan + " :Cannot join channel (+i)\r\n")
# define ERR_BADCHANNELKEY(client, chan)	(":" + g_host + " 475 " + client + " " + chan + " :Cannot join channel (+k)\r\n")
# define ERR_CHANOPRIVSNEEDED(client, \
								chan)	(":" + g_host + " 482 " + client + " " + chan + " :You're not a channel operator\r\n")

# define ERR_UMODEUNKNOWNFLAG(client)	(":" + g_host + " 501 " + client + " :Unknown MODE flag\r\n")
# define ERR_USERSDONTMATCH(client)		(":" + g_host + " 502 " + client + " :Cant change mode for other user\r\n")

# define	ERR_ISFOUNDER(client, nick)		(":" + g_host + " 701 " + client + " " + nick + " :Couldn't perform action because user is founder\r\n")
# define ERR_TOOLONG(client, size, limit)(":" + g_host + " 702 " + client + " " + size + " :size too long (limit is " + limit + ")\r\n")

# define NICK(sender, newn)				(":" + sender + " NICK " + newn + "\r\n")
# define PRIVMSG(sender, receiver, msg)	(":" + sender + " PRIVMSG " + receiver + " :" + msg + "\r\n")
# define JOIN(sender, target)			(":" + sender + " JOIN :" + target + "\r\n")
# define INVITE(sender, target, chan)	(":" + sender + " INVITE " + target + " " + chan + "\r\n")
# define TOPIC(sender, chan, topic)		(":" + sender + " TOPIC " + chan + " :" + topic + "\r\n")
# define MODE(sender, target, mode, arg)	(":" + sender + " MODE " + target + " " + mode + " " + arg + "\r\n")
# define PART(sender, chan, reason)		(":" + sender + " PART " + chan + " :" + reason + "\r\n")
# define KICK(sender, chan, nick, reason)(":" + sender + " KICK " + chan + " " + nick + " :" + reason + "\r\n")
# define QUIT(sender, reason)			(":" + sender + " QUIT :" + reason + "\r\n")
# define PONG(token)						(":" + g_host + " PONG " + g_host + " " + token + "\r\n")
# define ERROR(reason)					("ERROR :" + reason + "\r\n")

#endif
