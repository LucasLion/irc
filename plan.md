
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




