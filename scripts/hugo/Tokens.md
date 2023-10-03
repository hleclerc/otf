Tokens
======

Continuation de ligne
---------------------

Actuellement, `\` sert à continuer une ligne. C'est sympathique, mais ça oblige à utiliser les `,`...

On peut le garder pour des raisons pratiques, mais il faudrait un moyen de dire qu'on vient compléter.

Prop: un \ en fin de ligne dit 
    f g \
            a
            b
        c # => c'est de nouveau pour f

    func gunc(
            a
            b
        )
        c # => c'est de nouveau pour f

    func gunc
        \   a
        \   b


Fonctions sans parenthèses
--------------------------

Prop: on dit que tous les tokens qui apparaissent sont des appels de fonction, même les variables 
    Du coup `return 10` et`return` fonctionnent pareil

Par conséquent, une variable, c'est une fonction qu'on appelle sans argument.

Dans la tokenisation, quand arrive une variable, on fait direct un `call(variable,...)`.

Rq: si une variable donne direct un Call(Variable), peut être que faire un token variable aurait un intérêt limité.

Un proposition pourrait être de dire que les noeuds sont en fait des types de call, éventuellement en faisant des call sans argument si on veut optimiser, mais ce n'est pas obligatoire.


Problèmes à résoudre:
* `return` sans argument devrait agir comme une fonction
* pour appeler une fonction sans argument, il faut utiliser les parenthèses `f()`, ce qui crée une incohérence

Une des solutions pourrait consister à dire que tout nouveau token est un appel de fonction (et qu'on peut ajouter des arguments)
    
Opérateur `:=`    
* `a := ...` ou `a( x, y ) := ` devriendraient cohérent.
* Par contre, il faudrait dire ce que `a( x, y ) := ...` signifie dans une classe. En particulier, `a() := ...` et `a := ...` signifierai la même chose

Fonctions
* Comment passer une fonction sans l'appeler ? 
* Est-ce que l'appel caché ne risque pas d'effrayer les gens qui s'inquiètent des coûts cachés ?
    * Une proposition pourrait être de mettre un paramètre pour 

Objets
* Écrire `Vec[ &PI32, ... ]` si on veut une solution pour ne pas construire le `PI32`, ça va être insupportable.
* Une solution pourrait être `PI32.new` 


