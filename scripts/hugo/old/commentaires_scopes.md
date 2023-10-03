Scopes
======

On aimerait éviter les "surprises" pour le lecteur, donc autant que possible, le lecteur doit pouvoir trouver d'un coup d'oeil _d'où viennent les variables_.

Dans cet esprit là, on peut
  * proposer des ajouts. Par exemple: `{ +sin } := import "foo.ppw"`
  * faciliter la spécification de surdefs. Par exemple: 
        ```python
        def pow( x, y, spec_funcs = {}, base_funcs = { exp, log } )
            funcs := base_funcs + spec_funcs
            funcs.exp y * funcs.log x
        ```
        
        ou

        ```python
        def pow( x, y, exp = exp, log = log )
            exp y * log x

        pow 2, 20, funcs... avec funcs = { exp, log }
        ```

        => dans le 2ème cas, funcs ne pourra pas contenir plus que demandé

    Rq de la mort: les valeurs par défaut ne pourront jamais aller plus loin que les définitions de base, que ça soit pour le print, ou plein d'autres fonctions élémentaires.

    => ça pousse à revenir sur le principe des `globals`.

Dans un autre extrême, on pourrait permettre d'aller chercher les variables dans les lib.

Dans ce cas, on fait comme si tous les exports de toutes les modules étaient disponibles dans tous les modules

Globals
=======

Les globals, c'est cool pour proposer plein de surdefs... mais on pourrait être tentés de tout mettre en global pour permettre les surdefs, et ça devient crad.

Autre proposition: on accumule les surdefs au fur et à mesure des appels. Quand on appelle une fonctions, on donne une liste de l'ensemble des surdefs disponibles depuis l'appelant. Cette liste sert ensuite à ajouter des surdefs si on utilise une fonction.

Autre façon de faire: on fait des `import "..."` qui ajoutent toutes les variables exportées.

=> c'est sûr que c'est moins ch*ant à utiliser...
=> lorsqu'on appelle une fonction, on utilise le scope complet de l'appelant... qui est tout simplement le scope du point d'entrée

Pour plus de sécurité on peut imposer des arguments du genre
  * `import { sin, cos } from "..."` ou
  * `import { sin, cos } in "..."` ou
  * `import { sin, cos }, "..."`

Pb: comment considérer le from ? 
  * Prop 1: un type d'argument spécial pour import, fonctionnant comme le for avec `in`. La différence, c'est que le `in` n'est pas optionnel avec le for.
     Rq: `for a := 1, a < 5, ++a` => la proposition, c'est que `in` crée des variables et renvoie une classe avec une ref sur la liste.
     Pour l'import 


Autre proposition: on impose de déclarer la première def, et c'est l'objet auquel il sera fait référence. L'objet est mutable : à chaque import qui propose une nouvelle surdef, l'objet la stocke de façon globale dans la Vm
  `default_def f`
  `base_def f`
  `base def f`
  `oridef f`
  `accdef f`
  `mutdef f`
  `bef f`
  `declfu f`

  `root_def f`

  `stem_def f`

  `s_func f a, b when a == b`
  `surdef f a, b when a == b`
  `def f a, b when a == b`

On pourrait aussi passer par des signes
  `f :: Decl`
  `f :+ ( a, b ) => ...`

Question: faut-il rendre explicite l'ajout de surdefs ? Non: ça apporte plus de complexité à la lecture, et surtout, il faudrait le faire aux endroits
  * `import { +cos } from "..."` ou

Rq: ça revient à déclarer puis définir plus tard


Declaration des variables standards:
  * operator module doit forcément être fait à la main
  * 
  * 



