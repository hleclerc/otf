Scopes intermédiaires
=====================

Scope est essentiellement un vecteur de Variables, avec `parent` qui sert pour `:=` ou autre qui envoient plus haut

Celà dit, on pourrait très bien faire un type d'argument pour traduire en pointeur sur variable, etc...

Quand on fait un call, ex `a := ...; f a, 10`, `def f( a, b ), p a, b`,
  * on remplit des PrepArg avec une mise en cache.
     On pourrait imaginer que le cache contienne des Variables, et que les PrepArg contiennent les Rc<Ref>
  * ensuite, on peut faire l'appel avec création d'un scope où les variables sont déplacées depuis le preparg
     Rq pour les conditions: 

  * on remplit le scope d'appel avec des références ou des variables créées pour l'occasion
    Rq: ce n'est pas vraiment un scope. C'est plutôt un vecteur string + Variable. On peut en interne faire un vecteur de variables
  *

Question: est-ce qu'on pourrait remplacer les scopes par des vector<Rc<Ref>> ?
  Ça voudrait dire qu'il y a forcément non-appartenance, alors que certaines variables ont été créées pour l'occasion
  A contrario, si on met des variables, il y a forcément appartenance

Prop: un flag dans Variable pour dire s'il y a appartenance...
  => ça ajoute de la complexité, juste pour éviter des inc ou dec ref qui sont sans effet autre que l'optimisation

Cependant, on a besoin d'avoir une vision précise du nombre de fois qu'une variable est référencée.

Transférabilité
===============

Basiquement, on pourrait avoir un flag `transferable` dans variable mais ça serait cool si ça pouvait être dynamique

Prop 1: on met un Kei ou (Keb) pour définir la transférabilité.
  => ça sera en lecture seul

Comment fonctionne le copie d'une Variable ?
  => on pourrait autoriser seulement le déplacement. La copie passerait par des fonctions spécialisées.

Rq: la destruction des variables (ou plutôt le dec_ref) interviendrait au dernier accès d'une variable.
  def f a
    print a => 
  a := 10
  f a => `a` est transféré à `f`. Idéalement, `f` devrait dire s'il peut prendre en charge la destruction de `a`. On pourrait imaginer aussi qu'une fonction impose l'appartenance, etc...
    Si `f` peut prendre en charge la destruction de `a`, il faut qu'il ait toute l'information sur la structure... Il faudrait logiquement que ça passe par un type.

Mais dans ces conditions, à quoi sert Ref ? Pour dire à quel Expr on en est.


Prop: ce sont les types qui gèrent l'owning. Quand on crée une variable (le type de base), on


Algorithme pour repérer la transférabilité
==========================================

Partant du niveau le plus haut et de la création de variables ou les catchs transférables, on pourrait ...

Prop: pour tous les calls, on cherche la dernière utilisation. Ça pourrait être dans une variable (si c'est retourné) ou dans un call, via les variables catchées.
  Si c'est dans les catchées, on marque le catch comme transférable et on continue de façon récursive.

Rq:
  * pour les else, on peut transférer 2 fois.


RQ DE LA MORT: pour le if/else, les variables créées sont partagées.
  Ça serait sans doute plus raisonnable de faire une transformation en représentation intermédaire.
  C'est dans cette représentation qu'on chercherait les variables créées ou catchées, avec des vecteurs dynamiques.
  On pourra ensuite créer un binaire ou en tous cas une représentation condensée.


Rq: dans un call standard, les arguments ne sont pas dans des scopes indépendants

Du reste, si on veut aller vers plus de finesse, on pourrait éviter de créer des scopes lorsqu'il n'y a pas de délai

On pourrait faire 
* un scope pour le module
* un scope pour chaque exécution différée
* une gestion explicite des destructions (lorsque les blocs n'ont pas leur propre scope) et des transferts.

Dans un if, on a une cond, qui peut créer une variable dans un premier scope, et 2 blocks qui ont chacun leur scope.

L'approche pourrait être de faire des scopes sans catch pour tous les arguments des calls, et de créer des scopes pour les arguments spéciaux.

Par défaut, les scopes des catch seraient à exécuter sans délai. Les arguments pourraient écrire dans ce scope ou dans le leur.

De base, on ne crée que des Var, Bin ou Call

Pb: on pourrait vouloir utiliser le scope d'un argument précédant.

Prop: le parser Vst renvoie un scope *

=> ça marchera pour les def

Pour `f( a, b ) := a + b`, on pourrait fonctionner de la même façon... Ça veut dire que les assignations pourraient accéder à des arguments a posteriori... Cependant, on pourrait différencier les variables simples des fonctions.



Différents types de blocks
==========================

* exécution différée
* blocks exclusifs
* blocks optionnels
* blocks répétables

Rq: on pourrait interdire le move des variables globales

Prop 1: on essaye de tout faire lors de la passe d'assignation des variables
  => lorsqu'un scope est répétable, les variables cessent d'être movable
  => lorsqu'on récupère une variable d'un scope, on récupère un pointeur et un flag pour dire si on a le droit de mover
    Dans un scope répétable, on peut faire une interface VariableUser pour récupérer les moves

    Dans des scopes exclusifs, on peut repérer les cas où il ne faut pas déplacer les moves. Pb: il faut aussi repérer les destructions de variables



Prop 2: dans une première passe, on peut chercher dans quels sous-scope on utilise une variable.
  => on a un bilan pour chaque variable d'où elles sont utilisées de façon récursive
  On va ensuite assigner les moves en fonction de chaque scope

Prop: les scopes contiennent des variable créées ou référencées et pointent sur les scopes enfants pour chaque utilisation
  À la fin, les variables contiennent un scope avec une unique référence

Lorsqu'on écrit les blocs exclusifs, on peut ajouter les variables à détruire en regardant les scopes où c'est utilisé.
  L'utilisation, ça pourrait être le scope* + un index dans le scope

Prop:
  VstVariableNode
    creation_scope    :: Scope *
    num_in_stack      :: PI
    provenance        :: VstVariable *
    usage             :: Vec[ VstVariable * ]

  VstScope
    created_variables :: PI
    variables         :: Map[ Str, VstVariable * ]
    catcher           :: Bool

Prop
  VstVariableHandler => add_use
  VstGlobals => 
  VstStack   => 
  VstScope   => push

Dans le fond, on veut faire un graph ou un arbre de variable

On avait dit qu'il fallait que les fonctions "spéciales" soient gérées via les arguments. Cependant, on pourrait aussi imposer d'avoir un seul type d'argument
  Pour `info`, ça pourrait être un block
  L'intérêt serait de 
    * simplifier les scopes
    * éliminer le pb des variables nommées

Proposition intermédiaire:
  * on travaille toujours avec des types spéciaux d'arguments, comme par exemple `def f arg: String..., `
  * qui permette de passer plusieurs arguments, et de les traduire en ce qu'on veut, à partir du scope du call
  * mais qui ne sortent pas de scope

Mais il reste à déterminer comment gérer les arguments nommés. En fait, le problème se pose déjà sans les fonctions spéciales
  
Exemple
  `def f a, b, c: String..., d` => on voudrait que ça soit le type d'argument qui s'occupe du nombre à prendre
    Cependant, si on présente un argument nommé, comment fait-on ? `f b:20, z: 170`
    Prop: on parcourt les arguments et on voit s'ils en veulent bien
    Par exemple `b:20` va coller dans le `b`, `z:170` va coller dans le `c`
      Du coup, le `d` dans `def f a, b, c: String..., d` ne sera jamais lu
  => dans Vst, il faudrait que les arguments spéciaux disent combien ils acceptent d'arguments

Question: que fait-on avec du spread ? Il semble que ça coince
  Prop: on l'interdit sur les fonctions spéciales


Le fond du problème, c'est qu'on veut pouvoir accepter des arguments nommés pour construire des varargs.

Si on ne tolérait que des arguments simples puis des arguments nommmés ou l'inverse, ça serait plus clair
  `f 10, 78, b: 50, c:5`
  `f b: 50, c:5, 10, 78`

mais ça revient à gérer un num_unnamed. Pour les arg spéciaux
  * si on les donne sans nom, on prend args[ num_unnamed ]
  * 

Prop: on regarde où on en est au niveau types arguments[ num_unnamed ]
  Si c'est nommé, on demande si le type accepte des arguments nommés
    * si oui, on balance, et on avance num_unnamed
    * si non, on cherche dans les arguments un nom identique
  Si c'est pas nommé, on 

Autre prop:
  Si le type num_unnamed accepte un nom, on balance direct et on augmente num_unnamed
  Si le type num_unnamed n'accepte pas de nom,
    * s'il n'y a pas de nom, on balance direct et on augmente num_unnamed
    * s'il y a un nom, on va chercher dans les arguments
