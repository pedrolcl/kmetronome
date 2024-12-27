% Index de l'aide

# Introduction

Drumstick Metronome est un métronome basé sur MIDI utilisant le séquenceur ALSA.

Le public visé sont les musiciens et les étudiants en musique. Comme tout physique
métronome c'est un outil pour garder le rythme tout en jouant de la musique
instruments.

Il utilise le MIDI au lieu de l'audio numérique, permettant une faible utilisation du processeur et très précis 
timing grâce au séquenceur ALSA. Cela signifie que vous avez également besoin d'un MIDI
synthétiseur pour la génération sonore, car le programme ne produit aucun son
par lui-même. Les meilleurs résultats proviennent de l'utilisation d'un synthétiseur matériel, mais vous pouvez
utilisez également des synthétiseurs logiciels à la place.

Demandez de l'aide, signalez les problèmes, les bogues et les demandes de fonctionnalités au système de suivi à l'adresse 
[Site du projet SourceForge](https://sourceforge.net/p/kmetronome/support-requests/)

# Utilisation du métronome Drumstick

## Caractéristiques

Vous n'avez qu'à ajuster certaines commandes, comme le curseur de tempo, le
la boîte de sélection des battements/mesure et le sélecteur de longueur de battement. Appuyez sur le bouton de lecture
commencer. Utilisez le bouton d'arrêt à votre convenance.

Le tempo peut être réglé de 25 à 250 QPM à l'aide du curseur. Les unités sont
quarts par minute (unités de métronome Mälzel). Vous pouvez également double-cliquer
sur la fenêtre principale pour ouvrir une boîte de dialogue où vous pouvez entrer un nouveau
tempo directement avec le clavier. Il y a aussi une zone de liste déroulante à choisir
et afficher le tempo en utilisant des noms musicaux italiens.

Beats/Bar peut être réglé de 1 à 32 temps. Ce sont le nombre de battements
sur chaque mesure ou mesure, et c'est le numérateur sur la signature de temps comme
ce serait noté.

La longueur du battement est le dénominateur sur la spécification de la signature rythmique,
et représente la durée de chaque battement. La modification de cette valeur ne
changer la signification des unités de tempo.

Motif est une liste déroulante permettant de choisir une définition de motif. Le défaut
La valeur « automatique » signifie que le programme génère des modèles en utilisant le
notes définies dans la boîte de dialogue de configuration (Fort/Faible) et le rythme
définition fournie par "Beats/Bar" et "Beat length". Il contient également
les noms des modèles définis par l'utilisateur.

## Commencer

Ce programme utilise le protocole MIDI, c'est donc une bonne idée d'avoir
quelques notions de base sur le MIDI afin de bien comprendre les concepts
derrière. Vous pouvez trouver ici une bonne introduction:
[Qu'est-ce que le MIDI](https://www.midi.org/midi-articles/categories/MIDI%201.0).

Drumstick Metronome produit des événements MIDI. Si vous voulez entendre les événements
traduit en sons dont vous avez besoin pour connecter le port MIDI OUT de ce
programme au port MIDI IN d'un synthétiseur MIDI. Il peut s'agir soit d'un
synthétiseur MIDI matériel ou logiciel. S'il s'agit d'un externe
synthétiseur matériel, vous avez également besoin d'une interface MIDI prise en charge par ALSA
installé sur votre ordinateur et un câble MIDI connecté à la fois au
l'interface MIDI de l'ordinateur et la prise MIDI IN du synthétiseur.

Si vous n'avez pas de synthétiseur MIDI externe, vous pouvez toujours utiliser le
programme avec un synthétiseur logiciel MIDI prenant en charge le séquenceur ALSA
connexions, comme [QSynth](https://qsynth.sourceforge.io). Commencez votre
synthétiseur logiciel avant Drumstick Metronome, puis effectuez la connexion
entre les deux programmes, soit dans la boîte de dialogue de configuration de Drumstick Metronome 
ou en utilisant un gestionnaire de connexion externe comme 
[QJackCtl](https://qjackctl.sourceforge.io).

N'oubliez pas d'installer une bonne [Sound Font](https://en.wikipedia.org/wiki/SoundFont) 
dans la boîte de dialogue "Configuration..." de QSynth.

## Configuration

Drumstick Metronome a des capacités de gestion de session limitées. il peut se souvenir
une connexion pour le port de sortie ALSA, et une connexion pour son
port d'entrée. Les connexions sont stockées à la sortie du programme et mémorisées
au démarrage. Vous n'avez pas besoin de cette fonctionnalité si vous préférez faire de tels
les connexions à la main, en utilisant aconnect ou tout autre utilitaire équivalent, ou
si vous utilisez un gestionnaire de session externe comme le patchbay inclus dans le
programme [QJackCtl](https://qjackctl.sourceforge.io).

Drumstick Metronome utilise un fichier de définition d'instrument au format .INS, le même
format comme Qtractor, TSE3, Cakewalk et Sonar. L'**instrument de sortie**
la liste déroulante permet d'en choisir un parmi les standards General MIDI,
Cartes de batterie Roland GS et Yamaha XG. Vous pouvez ajouter plus de définitions en créant
un fichier nommé `drums.ins` dans `$HOME/.local/share/kmetronome.sourceforge.net`. Les
contenu des listes déroulantes **Banque**, **Programme**, **Faible** et **Note forte** 
dépendent aussi de cette définition.

**Canal** est généralement 10, ce qui signifie le canal de percussion d'un General MIDI
synthétiseur. Il doit s'agir d'un nombre entre 1 et 16.

**Résolution** est le nombre de ticks (unités de temps) pour chaque noire.
Plage de valeurs de 48 à 960. La valeur par défaut est 120.

**La durée de la note** est la longueur (en nombre de ticks) de la période
entre une NOTE ON et son événement NOTE OFF correspondant. Ce contrôle est
activé uniquement lorsque **Envoyer les événements NOTE OFF** est également activé. Valeurs très faibles
peut provoquer des clics assourdis sur certains synthétiseurs.

Les sons de percussion n'ont généralement pas besoin d'événements NOTE OFF pour être envoyés après
chaque NOTE ON. Cochez la case **Envoyer les événements NOTE OFF** uniquement si votre
synthétiseur ou instrument prend en charge ou nécessite ce réglage.

**Bank** et **Program** sont utilisés pour changer la batterie des instruments
prenant en charge plusieurs paramètres. De nombreux synthétiseurs ne comprennent pas le programme
changements pour le canal de percussion.

En mode motif **Automatique**, le son **Note forte** est joué comme premier temps 
dans chaque mesure, tandis que tout autre temps dans la même mesure est joué en utilisant 
le son **Note faible**. Les valeurs numériques 33 et 34 sont les sons GM2 et XG
pour le clic du métronome et la cloche du métronome respectivement.

## Éditeur de motifs

En utilisant cette boîte de dialogue, vous pouvez éditer, tester et sélectionner des modèles. Créer
nouveaux modèles, vous enregistrez simplement la définition actuelle sous un nouveau nom.
Les motifs sont représentés par un tableau. Les lignes du tableau correspondent à
les sons de percussions. Vous pouvez supprimer et ajouter des lignes à partir d'une liste de sons
défini par les paramètres de l'instrument dans la boîte de dialogue de configuration. Les
le nombre de colonnes du tableau détermine la longueur du motif,
entre 1 et 99 éléments de n'importe quelle longueur de battement.

Chaque cellule du tableau accepte des valeurs comprises entre N=1 et 9, correspondant au
Vélocité MIDI (N*127/9) des notes, ou 0 pour annuler le son. Valide
les valeurs sont aussi f (=forte) et p (=piano) correspondant à la variable
vitesses définies par les boutons rotatifs (Fort/Faible) dans la fenêtre principale.
Les valeurs des cellules peuvent être sélectionnées et modifiées à l'aide du clavier
ou la souris. Il n'est pas nécessaire d'arrêter la lecture avant de modifier le
cellules.

# Référence de commande

## La fenêtre principale

### Le menu Fichier

**Fichier → Importer des modèles**

: Importe les définitions de motifs dans Drumstick Metronome

**Fichier → Exporter les modèles**

: exporte les définitions de motifs depuis Drumstick Metronome

**Fichier → Lecture/Arrêt**

: Contrôle la lecture du motif

**Fichier → Quitter**

: Quitte Drumstick Metronome

### Le menu Édition

**Modifier → Modifier les modèles**

: Ouvre l'éditeur de modèle

### Le menu Paramètres

**Paramètres → Afficher les boutons d'action**

: affiche ou masque les boutons d'action. Il existe des éléments de menu équivalents pour chaque bouton.

**Paramètres → Afficher la barre d'outils**

: affiche ou masque la barre d'outils. Il existe des éléments de menu équivalents pour chaque bouton.

**Paramètres → Configuration**

: Configure le métronome de la baguette

### Le menu d'aide

**Aide → Contenu de l'aide**

: Ouvre la fenêtre d'aide

**Aide → Langue**

: Ouvre un sous-menu affichant les traductions disponibles

**Aide → à propos**

: Ouvre une boîte à propos de ce programme

**Aide → à propos de Qt**

: Ouvre une boîte à propos des frameworks Qt

# Contrôle externe

Drumstick Metronome peut être contrôlé à l'aide de son interface D-Bus, System Realtime
et messages MIDI exclusifs au système.

## Interface D-Bus 

Drumstick Metronome fournit certaines fonctions via son interface D-Bus. Vous pouvez
utiliser un programme compatible D-Bus pour contrôler le métronome de baguette comme `qdbusviewer` ou le 
utilitaire de ligne de commande `qdbus`.

Par exemple, ces commandes peuvent être utilisées à partir d'une invite shell :

    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.play
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.stop
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.cont
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTempo 150
    $ qdbus net.sourceforge.kmetronome-23324 / net.sourceforge.kmetronome.setTimeSignature 3 8

## Messages universels exclusifs au système

Drumstick Metronome comprend certains messages Universal System Exclusive. Parce que
l'ID de l'appareil n'est pas encore implémenté, tous les messages reconnus doivent
être marqué comme diffusé (0x7F).

Message en temps réel : Message de modification de la signature horaire

    Format : 0xF0 0x7F 0x7F 0x03 Longueur de commande Numérateur Dénominateur ... 0xF7
                                0x02 : Modification de la signature temporelle 
                                0x42 : changement de signature de temps retardé

Remarques : les deux commandes n'affectent que les prochains modèles programmés après
réception de la commande. La longueur est d'au moins 2. Octets supplémentaires suivant
Le numérateur et le dénominateur sont ignorés.

Messages MMC

    Format : 0xF0 0x7F 0x7F 0x06 Commande 0xF7
                                0x01 : Arrêter
                                0x02 : jouer
                                0x03 : jeu différé

## Interface en temps réel du système MIDI

Vous pouvez utiliser un appareil MIDI générant des messages MIDI System Realtime pour
contrôle Drumstick Métronome. Les messages système en temps réel acceptés sont : Démarrer (0xFA),
Arrêter (0xFC) et continuer (0xFB).

De nombreux claviers maîtres MIDI et appareils à boutons MIDI fournissent ces trois
Contrôles de transport MIDI. Vous devez connecter vos appareils MIDI externes
à l'ordinateur à l'aide de câbles MIDI (ou USB) et connectez le
port ALSA correspondant au port d'entrée de Drumstick Metronome. Cette connexion
doit être fait à la main, ou à l'aide d'un logiciel de gestion de session comme
QJackCtl.

# Crédits et licence

Programme Copyright © 2005-2024 Pedro Lopez-Cabanillas

Documentation Copyright © 2005-2024 Pedro Lopez-Cabanillas

Ce programme est un logiciel libre ; vous pouvez le redistribuer et/ou modifier
sous les termes de la licence publique générale GNU telle que publiée par
la Fondation du logiciel libre ; soit la version 2 de la Licence, ou
(à votre choix) toute version ultérieure.

Ce programme est distribué dans l'espoir qu'il sera utile,
mais SANS AUCUNE GARANTIE ; sans même la garantie implicite de
QUALITÉ MARCHANDE ou ADAPTATION À UN USAGE PARTICULIER. Voir le
Licence publique générale GNU pour plus de détails.
                                                                        
Vous devriez avoir reçu une copie de la licence publique générale GNU
avec ce programme. Sinon, voir [https://www.gnu.org/licenses](https://www.gnu.org/licenses)

#Installation
 
## Comment obtenir le métronome Drumstick

Ici vous pouvez trouver la dernière version: 
[Fichiers du projet](https://sourceforge.net/projects/kmetronome/files/kmetronome/)

## Conditions

Pour compiler et utiliser avec succès Drumstick Metronome, vous avez besoin de Qt 5 ou
plus tard, Drumstick 2, les pilotes ALSA et la bibliothèque ALSA.

Vous avez également besoin d'un synthétiseur MIDI pour produire du son.

Le système de construction nécessite [CMake](https://cmake.org) 3.14 ou plus récent.

La bibliothèque ALSA, les pilotes et les utilitaires peuvent être trouvés sur 
[Page d'accueil ALSA](https://www.alsa-project.org).

Drumstick::ALSA est un wrapper C++ autour de l'interface du séquenceur ALSA utilisant Qt
objets, idiomes et style. Vous pouvez le trouver au
[Page d'accueil de Drumstick](https://drumstick.sourceforge.io).

Voir aussi la [page d'accueil de Drumstick Metronome](https://kmetronome.sourceforge.io/kmetronome.shtml).

## Compilation et installation

Pour compiler et installer Drumstick Metronome sur votre système, tapez le
suivant dans le répertoire de base de la distribution Drumstick Metronome :

    % cmake .
    % Fabriquer
    % sudo faire l'installation

Étant donné que Drumstick Metronome utilise `cmake` et `make` vous ne devriez avoir aucun problème
le compiler. Si vous rencontrez des problèmes, veuillez les signaler au
l'auteur ou le projet 
[système de suivi des bogues](https://sourceforge.net/p/kmetronome/bugs/).

