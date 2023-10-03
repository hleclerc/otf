Construct
=========

Prop: construct sert à initialiser un objet étant donné sa position en mémoire (matérialisée par self)

Questions:
* comment dire dans un construct utilisateur si tel ou tel attribut doit être pré-initialisé ?
    Prop 1: on passe par un argument nommé pour chaque attribut. Dès qu'on arrive sur un argument non nommé, on complète les attributs non définis
        sdef construct
            a: [ 10, e: 20 ]... # mettre ... est une façon de dire que c'est un série d'arguments. C'est un peu relou
            b: 17               # Rq: utiliser les : pour la construction d'attribut bloque pour cette def comme toutes les autres les : pour autre chose (comme par exemple pertinence, when ?)
            c:. 
    Prop 2: on passe par un argument avec un nom spécial
        sdef construct
            init:
                a.construct 10, e: 20
                b.construct 17
            # => on voit ce qui a été construit
    Prop 3: on trace les call construct. Quand il y a besoin d'une valeur, on appelle le constructeur
        sdef construct
            a.construct 10, e: 20
            b.construct 17
            info c # on vient récupérer l'expression de c
        On pourrait imaginer que le self passé au construct soit en fait un proxy qui crée des proxy pour les attributs, qui
            * construisent avec les valeurs par défaut lorsqu'on fait un get
            * marquent la construction lorsque construct est appelé (on pourra tester si c'est appelé une fois)


* quelle méthode appeler pour gérer la réservation mémoire, sur la pile, dans le tas ou via un pool ? def allocate ... ?
    