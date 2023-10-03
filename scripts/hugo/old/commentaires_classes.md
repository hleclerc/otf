Plusieurs possibilités pour décrire les arguments dans une classe
  1. class A       # on éxécute et on regarde ce qu'on a dans le scope
       a := 10    #  
       b :: PI32  # 

  2. class A     # on fonctionne comme les arguments d'une fonction. Le CR pourrait ajouter des enfants au A
       static f := 50
       a = 10    # valeur par défaut
       b : PI32  # contrainte sur le type
       def m ... # on peut imaginer que decl parse séparemment les def, et stocke séparément les méthodes à des endroits différents

    En synthétique: class( A( a = 10, b : PI32 ) )

    Rq: le decl des classes pourrait prendre les arguments qui suivent
    Ou alors, on pourrait imaginer que Decl prend tous les arguments qui suivent, y compris pour les defs. Ça s'appliquerait aussi à `:=`, mais ça ne devrait pas poser de problème

    Rq: de base, dans l'analyse du body de la classe, on a des noms pour dire de quel argument on parle. Si on se contente du `:`

    class A 
       static f := 50 # static fera qu'on ne créra d'argument dans le body
       a : 10    # 
       b : PI32  # 
       def m ... # 
       class C   # Attention: ce n'est pas statique

    => on pourrait imaginer que seuls les arguments nommés peuvent créer des attributs

    Rq: on est sur une incohérence avec les defs. `def smurf` va créer `smurf` sur le scope, comme `a := ...`. Pour être cohérents avec les defs et classes, il faudrait utiliser `:=`.
      Ou alors on dit que := sert à créer des variables intermédiaires, ou statiques

        class A            # 
            a := 10        #  
            b : PI32 = 50  # 
    
    
    Dans l'approche `:=`, on a un problème de confusion entre ce qui permet de trouver le type, et ce qui permet de construire l'objet

On aimerait avoir cohérence entre notations
  * f arg: ...         # pour dire quels arguments on donne
  * def f( arg : ... ) # pour dire le type attendu
  * def f( arg = ... ) # pour donner la valeur par défaut
  * class A, a := 0    # pour donner les attributs

On pourrait assembler
  * ce qui donne les valeurs
  * ce qui donne les types

Exemple:
  * f arg: ...           # pour dire quels arguments on donne
  * def f( arg ~ ... )   # pour dire le type attendu
  * def f( arg: type = ) # pour donner la valeur par défaut
  * class A, a: 0        # pour donner les attributs

`:` pour création de variable nommée
`=` pour réassignation

Que se passe-t-il lorsqu'on fait des assignations dans une déclaration ou dans un appel ?
  func a: 10, b := 52, ...

Autre prop: on laisse tomber `:=` au profit de `:` pour donner une _valeur_
  Exemple
    * `f arg: ...` # argument nommé
    * `def f val: 0` # valeur par défaut
    * `class A val: 0` # valeur par défaut
  Pour définir un filtre de type ou déclarer une variable avec le type
    * `def f val :: PI32` # on veut exactement ce type
    * `def f val :~ PI32` # on veut exactement ce type
    * `def f val :· PI32` # on veut un type qui se comporte comme
    * `def f val :. PI32` # on veut un type qui se comporte comme
    * `def f val :> PI32` # on veut un type qui se comporte comme
    * `def f val :| PI32` # on veut un type qui se comporte comme
    * `def f val :* PI32` # on veut un type qui se comporte comme
    * `def f val :/ PI32` # on veut un type qui se comporte comme
    * `def f val ~ PI32` # on veut un type qui se comporte comme
    * `def f val « PI32` # on veut un type qui se comporte comme
    * `def f val @ PI32` # on veut un type qui se comporte comme
    * `def f val » PI32` # on veut un type qui se comporte comme
    * `def f val • PI32` # on veut un type qui se comporte comme
¹~¬#{[|`\^@]}¤¨`^ ̣·•´”łµ^`

L'intérêt de ne pas utiliser `:` pour donner les types est de spécifier des relations plus précises. On pourrait cependant arriver au même résultat avec des fonction. Ex: `def f a: Exactly[PI32]`

La spécification des types est en fait la spécification de filtres. Dans ce sens, il faudait plutôt un signe du genre `|`
   `def f a | PI32, b | Exactly[A]`

Pb: le signe | est trop "intrusif" et sert normalement au "ou"
   `def f a | PI32 : 10, b | Exactly[A]`
   `def f a » PI32 : 10, b » Exactly[A]`
   `def f a « PI32 : 10, b « Exactly[A]`
   `def f a ? PI32 : 10, b ? Exactly[A]`
   `def f a · PI32 : 10, b · Exactly[A]`
   `def f a • PI32 : 10, b • Exactly[A]`
   `def f a @ PI32 : 10, b @ Exactly[A]`
   `def f a \ PI32 : 10, b \ Exactly[A]`
   `def f a ^ PI32 : 10, b ^ Exactly[A]`
   `def f a ~ PI32 : 10, b ~ Exactly[A]`

Si : donne le filtre
   `def f a : PI32 = 10, b = 10`
   `def f a : PI32 | 10, b | 10`
   `def f a : PI32 ? 10, b ? 10`
   `def f a : PI32 · 10, b · 10`
   `def f a : PI32 • 10, b • 10`
   `def f a : PI32 \ 10, b \ 10`
   `def f a : PI32 ^ 10, b ^ 10`
   `def f a : PI32 ~ 10, b ~ 10`

Pb: en fait, dans `def f a`, `a` ne doit pas être analysé de la même manière que dans `f a` tout seul. Pareil pour les attributs
  => on est obligés d'avoir des listes spécialisées
    * appel => 
       `: ...` argument particulier
       `:= ...` interdit
       `:: ...` interdit
    * declaration => 
       `: ...` filtre
       `= ...` valeur par défaut
       `:= ...` interdit
       `:: ...` interdit
    * attributes => 
       `:= ...` variable avec une valeur
       `:: ...` variable avec un type
       `: ...` comportement
    * delayed (if, def, ...) => 
       `:= ...` variable avec une valeur
       `:: ...` variable avec un type
       `: ...` comportement


Pour clarifier, on aurait peut être intérêt à bien séparer les usages plutôt qu'à tout mettre dans le même panier. Dans ce sens, on pourrait réserver des opérateurs pour chaque usage.

* Arguments d'appel (cas générique)
    `func a: 10, 20, ...` =>     
* Déclaration + block (`def`, `class`) avec délai, qui crée une variable dans le scope parent
    `class A, a := 20, b := 30, def smurf, a` => `b` doit être préservé. On aura besoin du scope avec les noms
    Pour les méthodes, de base `def smurf a` va capturer le `a` qui sert à la définition
      * prop 1: dans une classe, le scope est spécial. Quand on va chercher une variables, ça devient `self.nom`

* Déclaration avec block (`for`) sans délai, qui ne crée pas de variable dans le scope parent
* Déclaration avec block (`:=`) sans délai, qui crée des variables dans le scope parent
* Block sans délai (`if`, `while`). Dans le cas du `if`, le premier token peut être exécuté directement, mais pas dans le cas du while
* StrAndVal pour `info` (on pourrait se base sur l'info de debug, à condition que les sources soient copiées integralement dans l'Ast... pourquoi pas)

On a donc pour chaque token à analyser
* des arguments de fonctions
* des déclarations, qui créent des variables dans le scope parent ou pas
* des blocks
  * où on peut créer des variables locales, qui seront lisibles à la fin du bloc ou pas (avec les noms et le positionnement des variables)
  * avec ou sans délai

`def f in ...`
`mdef f ...`
`sdef f( a, b ) ...`

Prop: pour les AstNode, plutôt que de faire un AstNode_Call qui prend un AstNode pour la fonction et un autre AstNode pour les arguments, on peu faire des AstNode_Call spécialisés
   AstNode_CallWithDeclarationAndArgsAndDelayedBlock  # `def f, ...` `sdef f, ...`
   AstNode_CallWithDeclarationAndArgsAndClassBlock    # `class A[] when ..., ...`,
   AstNode_CallWithArgsAndImmediateBlock              # `for ( a: PI32, b = 50 ) in ... when ..., ...`
   AstNode_CallWithImmediateBlocks                    # `if ...`, `if ... else ...`, `while`, 
   AstNode_CallWithArguments                          # `func a: 10, 20, ...`
   AstNode_Variable                                   #
   AstNode_Binary                                     #

Pour les `:`, il vaudrait mieux que ça soit géré au niveau du parsing (éviter des renvoyer des variables intermédiaire et de tester les types). Pour les `:=`, c'est comme pour le `def`, c'est une déclaration... cependant, dans les classes, il y a la question du `self`. On pourrait peut être se baser sur un flag delayable pour dire qu'il faut remplacer les variables par des `self.`.


Allocation
==========

On voudrait trouver les types template non spécifiés

class A[ T ]
  def construct v: T # A( 10 ) => T = SI32
    ...

Pb: avant d'avoir T, on n'a pas parcouru la classe, on ne sait pas ce qu'il y a dedans, on ne connaît pas les constructeurs, etc...

Propositions:
  * on fait un parcourt des VstNodes pour faire une fonction qui trouve les paramètres qui manquent
  * on fait un premier parcourt, uniquement pour récuperer les constructeurs
    À ce compte là, ça pourrait faire les parcourt des AstArgs

L'idée de départ, c'était de confier le choix des paramètres templates à l'allocateur... mais ce n'est peut être vraiment son boulot.

Prop: on cherche tous les types possibles en analysant les Args

Ensuite, pour tous les types possibles, on récupère les Preps pour construct.

Pb: construct à besoin d'un self. Cependant, 











