Résumé du format
================

En frontal, EpType, c'est un pointeur sur les données d'un type.
  class EpType
    type_ptr :: EpExprOff # les données du type du pointeur
    data_ptr :: EpExprOff # les données du pointeur

Pour les données du type du pointeur, on veut surtout être capables de faire un deref et la gestion de la durée de vie.
  On pourrait imaginer que ça soit une succession de Def, potentiellement séparées directement. Par exemple:
    class EpType
      copy_ptr :: EpExprOff # Def pour dire comment copier le pointeur
      free_ptr :: EpExprOff # Def pour dire comment libérer...
      data_ptr :: EpExprOff # les données du pointeur

Autre solution: on fixe le type de pointeur.
  A priori, on devra gérer des graphes. On va se retrouver un peu partout avec des inc et dec refs.
  Sur quel type ça pointe ? On pourrai imaginer 
    class Type
      attributes :: Vec[ TypeAttribute ]

  À ce compte-là (si on dit que Type c'est une empreinte mémoire bien précise), on pourra faire des instruction spéciales pour créer des types, avec des méthodes pour accélerer la recherche.

  Rq: certains types auront des variables à stocker (valeurs par défaut).
    Une solution pourrait être de stocker les types dans des zones mémoire, à côté du contenu.
    On pourra éventuellement faire un pointeur (par exemple unique) sur le duo type + valeur pour avoir une taille constante en mémoire.




  Rq: on aura besoin de méthodes spéciales pour l'interpréteur, par exemple pour convertir en entier. A priori, ça consiste à récupérer du binaire.

    


EpType, c'est
  * un EpInt pour donner le type du pointeur vers le type.
    * on a besoin d'un pointeur parce que le type est partagé par plusieurs objets et il faut un moyent de gérer la durée de vie
    * il faudra être capable de faire une comparaison vers les types de base lorsqu'on demande la valeur du pointeur
      * l'idée pourrait être de mettre une classe qui dit d'où viennent les entiers comme argument de ct_value
  * un EpExprOff pour le contenu du pointeur
    * Les données pointées servent s'il y a des données à stocker. On pourra donc ne pas le remplir pour les types élémentaires

EpType permettra de retrouver CtTypeInfo du pointeur vers l'entier qui donne le CtTypeInfo.

A priori, le type num à mettre dans EpType, c'est celui du pointeur sur le type. On veut que quand on déréf, ça donne type + data compatible avec Type, où on peut trouver type_info_num dans les attributs 


L'idée pourrait être de donner le type qui pointe vers un `Type`.
  Le déréférencement permettrait d'obtenir une ref vers un `Type`.
  Et on pourrait gérer la durée de vie, etc...


Rq: TypeInfo pourrait aussi être Rt dépendant vu qu'on a besoin de stocker des Types...
  Éventuellement, on pourrait dire que tel ou tel sous-type va chercher ses variables dans la liste des variables capturées, mais ça signifie que les sous-type devront être recrées pour chaque assemblage qui en sont faits.


Pour simplifier, est-ce qu'on pourrait revenir sur le principe d'avoir un pointeur sur un type, éventuellement avec héritage ?
  class MyType
    def get_attribute name
      ....
    call_surdef :: PI 
    ... 
  Rq: si on veut stocker des références ou des copies de variables, il faudra bien qu'on soit capables de gérer des types différents.

EpType contient un pointeur sur des données de type. À partir de ces données, on peut
  * trouver les attributs, qui peuvent éventuellement contenir des surdefs, etc...
  * trouver la taille, etc...

On pourrait aussi jouer sur le cache. Par exemple, le type SI32 contient au début très peu de données, mais on dit que le cache contient des infos pour un type qui a le nom "SI32" et est "primitif" (à définir de façon précise), ou plus simplement s'il y a un entier non nul pour dire que c'est primitif.

  Rq: on s'embête avec les pointeurs, mais on pourrait aussi inclure les premières infos dans une zone mémoire d'Inst.


Quand on demande deref sur un pointeur, on va en faire chercher un attribut qui pourra être une méthode.
  => on peut faire un find_attribute pour les types.
    Retourne une variable
    Avec une variable self optionnelle en argument

Pb: on veut faire un deref du type pointé (à partir notamment de RawPtr[ T ]), et ça ne peut pas passer par une EpVariable... 
  On pourrait imaginer prendre une def statique histoire d'éviter de définir un self
  Par ailleurs, on ne pourra pas renvoyer la def sous forme de EpVariable
  Une solution pourrait être de mettre un type spécial de function, à stocker comme attribut spécial


Rq: dans RawPtr[ T ], T devra pouvoir être exprimé comme un EpType.
  
Quelles sont les données du type pour un RawPtr[ T ] ?
  * on aura un argument template pour T, et on sera emmenés à gérer la durée de vie, sauf éventuellement pour les RawPtr les plus courants qu'on pourra .

Pb: quand on fait deref_num sur le type de pointeur utilisé, on aimerait que ça puisse générer un ct_catch
  * a priori, les ct_catch fonctionnent avec les Refs qui donnent la provenance d'une variable.
  * mais quand on demande le numéro de la fonction de deref, 
    


Pourquoi a-t-on crée EpType ? 
=============================
  * ça permet de prendre des types conditionnels. Est-ce qu'on en veut vraiment ?
    * ça peut être pratique pour des trucs du genre a ? b : c
  * on pourra avoir accès aux données qui l'ont construit en runtime

  * l'idée est de partir des données sur un type et c'est depuis une table depuis `vm` qu'on peut retrouver le `Type *`. Quelles sont les données rt ?
     * l'origine ? Ça pourrait être une `Def` mais pas forcément. 
     * la description des attributs, avec leur type
     * les variables catchées, avec leur type...

  * question: est-ce qu'on va toujours créer un `Type *` pour tous les types utilisables ?
    * éventuellement, on pourrait avoir des craintes pour les variables catchées: on va pas créer un type pour chaque appel
    * l'approche pourrait être d'avoir un `TypeInfo *` pour les données génériques intangibles (attributs, auto-ref, etc...), éventuellement avec une liste des `TypeInfo *` pour les types catchés.
        + une ExprOff qui pointe vers une collection de pointeurs vers les variables catchées, qu'on pourra répercuter dans la variable.

  Prop: 
    * un type, c'est 
      * un nom
      * une liste d'attributs (statique, dynamique, etc...)
        * Ça pourrait être une liste, avec éventuellement une méthode pour lister des attributs supplémentaire ou renvoyer des attributs 
      * Si ça vient d'une classe, la classe d'origine la liste des paramètre templates
          Rq: si on veut des filtres du genre `obj : Cl[ PI8, T ]` avec `T` un paramètre libre on aura besoin de la def d'origine, des paramètres template, etc...

    * on stocke dans EpType 
      * le `TypeInfo *` qui décrit le pointeur vers le type (qui dit comment copier, detruire et déréférencer le pointeur)
      * l'`ExprOff` du pointeur, optionnel

    Rq de la mort: si on veut faire des ct_conds en fonction du type ou de valeurs qui viennent du type, il faudrait qu'on évite les pointeurs. On pourrait imaginer que vienne d'entier.

    Rq: on pourrait faire des ct_tests d'égalité qui prennent en compte le changement de valeur d'une compilation à une autre. Il suffit de dire d'où vient l'entier de référence (par exemple, que c'est l'entier qui correspond à telle ou telle variables, etc...).

    Du coup, EpType, ça serait un ExprOff vers un entier qui décrirait le pointeur vers le type
      on pourra déréférencer le pointeur pour aller chercher des infos sur le type
        notamment des entiers pour pointer vers des fonctions primitives du genre call


    

    


Prop: 1 seul type de Ref, avec des Types différents.
====================================================

Si on manipule une ref (ex: `a.b.c`), on a un type d'accès à une sous-partie: 
  * les données sont celles du `a`
  * on dit que ça peut être vu comme le type de `c`
  * une copie va donner le type de `c`


Rq: dans les PrepCall, on stocke des références. On pourrait cependant faire un type dont la copie ferait des ref effectives.


Gestion des inc_refs
====================

Pour les modules, on pourrait vraisemblablement renvoyer un `Rc<Ref>` sachant que les modules sont stockées en variables globales.

Pour les autres fonctions on aura des cas où les variables sorties ne seront pas des références.

Prop: Variable contient un Type et un `Rc<Ref>` qui ne contient qu'une expr avec comptage de référence

Rq: dans `def f( c, d ) z := c... f b, b` => il faut qu'on ait un moyen de savoir que `b` est utilisé 2 fois.
  => Ça veut dire que passer une variable en argument doit bien augmenter son compteur.

Prop: c'est le type qui dit si une variables est une référence d'un truc défini ailleurs.
  Rq: 


Arguments nommés lors du parse
==============================

f a: 10, 20, ... => on pourrait analyser le nom et le spread avant de passer au ArgType.

  Si 

* passage de la dernière référence
  Rq: dans un if/else, on devrait avoir la transférabilité dans les 2 blocs
  Rq: quand il y a exécution en différé, on crée de l'asymétrie
    a := 10
    def f
      a
    def g
      a
    => a sera envoyé dans g... Cependant la référence sera > 1 donc ça ne compte pas.
