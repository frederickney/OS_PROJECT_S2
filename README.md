Format des requêtes :
------------------------


        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |        Magic header separator Signature=0x76767676            |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                          Type                                 |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       +                          Handle                               +
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                          Offset                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                          Length                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       .                          Payload                              .
       .             Présent seulement si type=CMD_WRITE               .
       .                                                               .



Format des réponses :
------------------------


        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |           Magic header separator Signature=0x87878787         |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                          Error                                |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       |                          Handle                               |
       +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
       .                          Payload                              .
       .           Présent seulement si réponse à CMD_READ             .
       .                                                               .



Descriptifs des différents champs du protocole :
------------------------


* Magic header separator Signature :

  Simplement un nombre bien connu sur 32 bits qui permet d'éviter
  le traitement de message qui ne commenceraient pas par cette signature protocolaire. C'est un indicateur
  de début de message puisque ceux-ci arrivent dans un flux d'octets dont on ne peut prévoir la
  vitesse d'arrivée. En clair : un read(), ne renverra pas forcément le nombre d'octets attendu et
  il faut avoir un buffer intermédiaire.

		0x76767676 = REQUEST
		0x87878787 = REPLY

* Type :

  Un entier sur 32 bits qui définit le type de la requête. Deux valeurs possibles :

         0x0 = CMD_READ
         0x1 = CMD_WRITE

* Error :

  Uniquement dans les messages de type réponse. Il indique le résultat d'une commande
  qu'un client à demander à effectuer, une valeur à zéro indique que la commande a été effectuée
  avec succès, une valeur négative indique une erreur, qui corresponds à la valeur qu'on trouverait
  dans errno si la requête était effectuée localement.

* Handle :

  32 bits qui identifient de manière unique une requête et qui permet
  aux clients d'identifier une reponse à une requête et donc de demander plusieurs requêtes en même
  temps avant de traiter leur réponse : c'est du pipelining.
  Dans une réponse, le champs Error indique le retour d'erreur/non erreur pour une requête dont le numéro
  de handle était identique à celui indiqué dans la réponse. L'initialisation du Handle pour une requête
  devra utiliser la fonction rand(3) de la librairie C associée à une seed initialisée à sur la valeur
  de l'heure locale du système du client.

* Offset :

  un entier sur 32 bits qui indique à partir de quel offset en octet doit
  commencer la requête d'écriture ou de lecture.

* Length :

  longueur des octets à lire ou à écrire qui suivent l'entête du protocole.

* Payload :

  Les données lues dans une réponse à une requête de lecture, les données à
  écrire dans une requête d'écriture. Dans une réponse à une requête d'écriture, le payload
  est vide, le message contenant uniquement un code d'erreur et le handle correspondant.

