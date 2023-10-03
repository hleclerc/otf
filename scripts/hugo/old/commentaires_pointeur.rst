Que contient le pointeur de référence ?
=======================================

On peut imaginer que si une variable n'a pas la possibilité de s'auto-référencer, un comptage de référence suffit.

Pour le cas général, il faut cependant prévoir quelque chose de plus flexible.

On devra être capables de trouver les parents puis de faire des ops du genre destroy sur des types inconnus.

Pour EpType, on pourra dire que c'est un ExprOff vers un Ptr[ Type ].

Prop:
    class Ptr[ T ]
        orig_obj :: RawPtr[ GraphObjAncestor ] # keep track of container object
        obj      :: RawPtr[ T ]                # attention: il faudra que RawPtr s'adapte aux alignements inférieurs à 8 bits, donc potentiellement en stockant un offset pour les bits.

    class GraphObjAncestor
        parents  :: Vec[ RawPtr[ GraphObjAncestor ] ]
        vtable   :: GraphObjFuncs # possibilité aussi d'utiliser une vtable authentique
        count    :: RawPtr[ PI ]


    class GraphObj[ T ] inherits GraphObjAncestor
        obj      :: RawPtr[ T ]

En pratique, on va directement créer un Ptr avec 2 refs. Orig_obj sera directement la ref du GraphObj avec
    class Ptr[ T ]
        orig_obj :: RawPtr[ _GraphObj ] # keep track of container object
        obj      :: RawPtr[ T ]         # attention: il faudra que RawPtr s'adapte aux alignements inférieurs à 8 bits, donc potentiellement en stockant un offset pour les bits.

    class _GraphObj
        fun_table :: RawPtr[ GraphObjVtable ] # 
        parents   :: Vec[ RawPtr[ GraphObjAncestor ] ]
        count     :: RawPtr[ PI ]
        obj       :: RawPtr[ T ]

    class GraphObjVtable
        destroy   :: FuncPtr
        copy      :: Def

Les vtables contiennent des infos spécifiques pour chaque type. 

EpType s'attend à avoir du Ptr[ Type ]. 
  * la vtable pour del/copy/... du pointeur vers orig ne sera à faire qu'une seule fois.
    * Rq: on pourrait faire Inst_Type.
  * Comment gérer les variables non définies ?

Contenu de Type 
  * a priori essentiellement des listes d'attributs
    * attributs dynamiques: 


Comment on dit que le type d'une variable est Undefined ?
  * si on dit que EpType stocke les données d'un Ptr[ Type ], on n'a pas beaucoup d'autres possibilités que de coller un nullptr
  * ou alors on dit que EpType donne le type du pointeur puis les données du pointeur 
        Le type du pointeur serait une instance autosuffisante de Type 
        Ça pourrait être directement la valeur. Pas besoin de passer par un pointeur vu qu'il n'y a pas besoin de gérer la durée de vie.

        Pour la plupart des variables, ça serait un Ptr[ Type ], ou un RawPtr[ Type ] pour les types basiques.

        Pour les variables non définies, on pourrait avoir un type Undefined.


Variables non définies mais à référencer 
========================================

