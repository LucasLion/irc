#!/bin/bash

# Serveur IRC et port
serveur_irc="irc.fdfnet.net"
port_irc=6667

# Nom d'utilisateur et canal
nickname="amouly"
nickalt="amoulyalt"
username="amoulyuname"
realname="amoulyrealname"
votre_channel="#chantest45687"

# Nom du fichier de journal
logfile="nclog.txt"

# Connexion au serveur IRC
nc "$serveur_irc" "$port_irc" > "$logfile" &

# Attendre un peu pour que la connexion soit établie
sleep 2

# Envoyez votre nom d'utilisateur et pseudo (modify as needed)
echo "NICK $nickname" | nc "$serveur_irc" "$port_irc"
echo "USER $username 0 * :$realname" | nc "$serveur_irc" "$port_irc"

while true; do
    # Lisez la sortie de nc et enregistrez-la dans nclog.txt
    nc -l -c 'echo "$REPLY" >> nclog.txt'

    # Si vous recevez un PING, répondez avec un PONG
    if [[ "$REPLY" == "PING "* ]]; then
        PING_RESPONSE="PONG${REPLY:4}"
        echo "$PING_RESPONSE" | nc "$serveur_irc" "$port_irc"
    fi

    # Attendez un moment avant de vérifier à nouveau
    sleep 1
    echo "NICK $nickalt" | nc "$serveur_irc" "$port_irc"
done