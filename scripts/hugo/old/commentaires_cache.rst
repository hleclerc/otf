Repérage des valeurs ct utiles pour la mise en cache
====================================================

C'est au moment où on demande les valeurs ct qu'il faut créer les graphes (puisqu'on veut stocker les opérations à faire sur les entrées).

La proposition, c'est qu'à chaque fois qu'on demande une valeur ct sur une Expr, on regarde dans les dépendances les instructions qui font des gestions de cache.

Rq: on connait les valeurs au bout des callbacks, avec des conditions dans Vm.
  Ça dira: si telle ou telle condition (runtime) est vérifiée, on a cette valeur.
  Si la condition ne dépend que de paramètres qui sont crées après l'interception de cache, il faut qu'on se débrouille pour que ça soit des valeurs à coller dans le graph pour faire un test.
    Par exemple, une valeur pour un `if`. Bin... dans ce cas, on pourra se contenter de donner le graph choisi en bypassant le if. À la fin, on aura plusieurs valeurs à tester à partir des mêmes entrées.

  En résumé, lorsqu'on fait une fonction, on teste tous les embranchements. Le conditions stockées dans Vm disparaissent de même que les `if`.

Pour tester on pourra faire la construction d'une fonction. 
  Inst_Function pourra être construite ou appelée.
  Une fonction prend en entrée les données des variables des arguments... si on les demande.
  Ça apparaîtra sur le scope: on pourra faire des variables qui pointent sur l'Inst_InpFunction. Inst_InpFunction pourra intercepter les demandes.
    Rq: il faudra faire pareil pour les variables globales. Ça encourage à passer par un pointeur dans Vm qui dit comment aller chercher les variables, notamment les variables globales. 

  La solution précedente pour les "catchs", c'était de regarder si les refs étaient d'une date antérieure à la date de début de catch. On pourrait simplifier en mettant des dates sur les outputs (mods ou outputs purs) plutôt qu'en passant par les refs.
      Rq: on a besoin des refs pour assigner l'Inst_Inp dans la valeur courante.
    Ça marchera pour les ifs où le but est de construire le graph d'expression. 
    Cependant, pour les fonctions, il faut qu'on dise *d'où viennent les expressions* (pas seulement les graphes à un instant donné) pour être capables de refaire la construction. 
      
  En pratique, les arguments et les variables catchées sont connues au moment de l'appel. Pour ces dernières, on pourra remplir le scope avec des variables qui contiennent des Expr qui disent "type de l'argument numéro 1", ...
    A priori, ça sera directement dans l'Inst_Inp qui contiendra les infos de provenance dans les outputs. 

    En gros, de base, seules les variables globales auront à être interceptées... 

En réalité, dans les graphs sauvés par le ct, 

