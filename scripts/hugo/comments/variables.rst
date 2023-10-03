Prop: 
  * on a un système pour gérer new delete, et qui fonctionne avec du ct exclusivement.
  * et un autre système pour savoir s'il faut appeler les destructeurs
    Variable pourrait contenir
      - TypedValue pointeur vers ParentData
        Ça pourrait être en mode liste chainée 
            (le but est uniquement de vérifier si un objet est utilisé quelque part)
            (si ce n'est pas utilisé quelque part, on pourra parcourir les enfants avec les types)
          nb_times_handled: un entier qui compte le nombre de fois où c'est utilisé "en dur" par le compilateur
          prev_in_parents:
          last_parent: qui pointe sur ParentData
      - TypedValue pointeur vers l'objet d'origine (l'owner)
      - TypedValue pointeur vers l'objet observé

    Rq: si c'est un variable globale, on pourrait faire des optimisations notamment pour éviter de mettre à jour le ref_count pour rien
         .. mais c'est juste une optimisation

    Question: comment sont gérés les arguments inconnus
      * si c'est du TL, on doit pouvoir passert le ptr 
      * si c'est du ext, on pourrait très bien faire un ParentData artificiel


Test de Variable: on aura besoin de fonction du genre operator +( ... )
  * 

Rq: les ref_count sont connus à la compilation, puisque ça ne peut être manipulé qu'au moment de la compilation.
  Cependant la liste des parents est RT...
  Éventuellement, on pourrait séparer les ref_count de la liste des parents pour faciliter la création de code où seulement ce qui est nécessaire reste.
    Mais bon, on pourrait aussi prévoir une optimisation plus générique de "crunch" de donnée: on vire ce qui n'est pas utilisé, on colle ce qui est alloué et désalloué en même temps, ...

Rq: pour la liste des parents, on aimerait quelque chose qui facilite le travail du compile time 
  On pourrait imaginer un simple vecteur avec taille, et réservation qu'on pourrait modifier a posteriori
  Cependant, supprimer un parent va créer des circuits compliqués... mais bon, ça sera le cas aussi pour les listes chainées 
    Éventuellement, pour éviter d'avoir à tester toutes les combinaisons de cas, on pourrait faire un truc pour repérer les cas indépendants
    Et du coup, faut-il nécessairement écrire dans des zones mémoire différentes ?

  L'idée de base serait d'avoir un pointeur vers un emplacement qui contient à la suite
    * le ref_count
    * la taille 
    * la réservation
    * de la place pour des pointeurs pour les premiers parents
    * une pointeur vers les vecteur extérieur

  Les parents, c'est le pointeur avec le type. Il faut qu'on soit capables de retrouver le ref_count et tout.

Remarques sur les dépendances des insts
  * on aimerait différencier les dépendances des arguments. Par exemple, on pourrait avoir besoin d'offsets pour réaliser une instruction, mais ce n'est pas une dépendance 
  * On pourrait reprendre le principe des slots E/S qui sont bien les arguments, mais les dépendances pourraient venir de fonctions qui parcourent notamment les slots 
    Attention pour les slots: on aimerait bien pouvoir stocker 
      - les données en dur ? Ça pourrait se faire avec un Inst_CtData
        C'est coûteux (6 PI/Ptr + bool + dtor complexe) mais bon, on peut faire avec, au moins dans un premier temps
      - l'offset ? Ça pourrait être une fonction calculée... Mais comme c'est une entrée, on pourrait très bien utiliser une Inst_SubContent si jamais l'offset est != 0
        À ce compte là, on pourrait dire que les inputs n'ont pas d'offset en général.
        Par contre, il en faudra bien sûr pour les mod
      - la longueur ? A priori, ça peut se calculer a posteriori.

Rq: à ce compte là, on pourrait aussi imposer que les inputs ne prennent que des insts à 1 sortie... mais ça se mord la queue 

Pb de la mort: travailler sur graph + une liste de parents => pas thread safe.
    

Question sur les instructions: quand on demande des valeurs comme par exemple des entiers ou des références, on peut avoir  
  * une seule valeur, sûre à 100%
  * pas de valeur 
  * un nombre limité de valeurs, associées à des conditions
  * un nombre super important, genre des propriétés (pair, positif, etc...).

Pour les valeurs conditionnées, on pourrait proposer une liste condition + Opt[ Valeur ]
  Si pour certaines conditions il est impossible de trouver une valeur, on pourra 
    * ou bien vider la liste 
    * ou bien mettre un flag ou une instruction pour certaines conditions. L'instruction sert à dire qu'on n'a pas la valeur.
      Ça fonctionne comme un KPI. Du coup, est-ce qu'on pourrait généraliser KPI pour par exemple Kn<...> (Known or Not) ?
        C'est tentant, mais il y aura le cas particulier du PI qui peut ne pas avoir la même taile sur la cible.
        On pourrait éventuellement faire un TPI comme Target Positive Int... mais en fait le pb est plus général vu qu'il y a le pb de l'endianness pour d'autres types. 
        

