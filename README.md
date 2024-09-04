# stage1A
code de mon stage de 1A à l'N7 - SN
Sujet : Mise en oeuvre d ’un protocole de synchronisation dans le système d’exploitation Contiki-ng

dans le repertoire "sisp_app" ce trouve plusieur dossier : 
- sisp_module 
    module contenant l'implémentation du protocole sisp
    /!\ le dossier sisp_module doit être ajouté au dossier os/services/ du repertoire contenant le code de Contiki-ng

- sisp_nullnet_iee
    un exemple d'utilisation du protocole sisp en utilisant la pile de comunication Nullnet de contiki

- sisp_iee_udp_mcast
    un exemple d'utilisation du protocole sisp en utilisant la pile de comunication classique de contiki

- sisp_ble_udp_mcast
    tentative d'implémentation du protocle sisp en utilisant la pile de communication BLEach 
    (n'arrive pas à établire de connexion entre les diffférentes cartes + impossible pour une carte d'à la fois ecouter et transmettre)

- test_udp__ble_client_server
    contient 2 dossier client et serveur et permet d'établire une connexion BLE entre les 2. 
    utilise en plus du code de contiki-ng, le code plus complet de BLEach issue de la pull_request suivante : https://github.com/contiki-os/contiki/pull/2312
    (utilisant le code issue de la pull_request : https://github.com/contiki-os/contiki/pull/1290) ces codes ont ensuite été adpaté a la syntaxe de contiki-ng
    (les fichiers modifiés ou ajoutés par ces pull_request sont présent dans le dossier modification_contiki_ng avec le même chemin que dans le repertoire contenant le code de contiki-ng)

- test_rpl_border
    contient 2 dossier base_node et border_router. correspond au "set-up" précisé dans le tutoriel correspondant de la docuùentation de contiki_ng (https://docs.contiki-ng.org/en/develop/doc/tutorials/RPL-border-router.html). 
    /!\ verifier avant de run la commande "make connect-router" que ipv6 est enable (à defaut, lancer la commande : 
    ```bash 
    sudo sysctl -w net.ipv6.conf.all.disable_ipv6=0
    ```
    )

- mqtt-client
    une "amélioration" de l'exemple mqtt-client (https://docs.contiki-ng.org/en/develop/doc/tutorials/MQTT.html) ajoutant notemant une extension pour le capteur de température des cartes cc2650stk et une tentative d'utliser le protocole sisp en paralèle et de publier sur le topic correspondant la valeur de l'horloge partagé a chaque transmission.
    pour lancer le broker mosquitto, utiliser une configuration qui défini a minima les variable suivante : 
        allow_anonymous true
        listener 1883

    ```bash
    mosquitto -v -c /etc/mosquitto/conf.d/mosquitto_test.conf
    ```
    où /etc/mosquitto/conf.d/mosquitto_test.conf est le chemin vers la configuration mosquitto.

    /!\ dans cette exemple, il semble y avoir un problème de coucurence entre les processus gerant le protocole sisp et celui gerant le processus du client mqtt
        en conséquence, le client ne parvient pas à recevoir le message PINGRESP du broker lors de la tentative de connexion à ce dernier.


