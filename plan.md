
# PLAN FT_IRC

## Besoins

### fonctionnalites principales

- Connexion avec TCP/IP
- Authentification du client
- definir un pseudo
- definir un nom
- rejoindre un salon
- envoyer et recevoir des messages prives
- les messages envoyes sur un salon doivent etre transmis a tous les autrse clients du salon
- avoir des moderateurs avec des commandes exclusives:
    - KICK (ejecte un client du serveur)
    - INVITE (invite un client dans un salon)
    - TOPIC (change ou affiche le topic du salon)
    - MODE (change le mode du salon):
        0- i: active/desactive Invite-only (salon prive?) channel
        - t: active/desactive les restrictions de la commande TOPIC des moderateursk
        - k: active/desactive le mode de passe du salon
        - o: accorde/retire les privileges de moderateur d'un salon
        - l: active/desactive la limite d'utilisateurs d'un salon

### bugs

- "You're not a channel operator" s'affiche a la connexion

### a faire

- il faut gerer le /who (la commande /who envoie a tous les users du channel et mal) [OK]
- /nick _nickname_ fait erroneus (devrait fonctionner) [OK]
- gerer les erreurs (throw etc...) [OK]
- verifier que ca fonctionne avec IPv6 [OK]
- il faut gerer le /leave / /join (messages bien distincts sur chaque channel)
- /topic (quand pas de topic) affiche dans le hub (pas bien)
- fermer le serveur proprement
- leaks

### bonus

- envoi de fichiers
- creation d'un bot
