Références
==========

En principe, les pointeurs pour les références devraient venir d'InstAlloc pour pouvoir les coller (les pointeurs) comme attribut.

On pourrait imaginer que les ct soient construits par copie sur les zones mémoires

    InstInitCtValue( InstDeref(  ), type, ct_value )

Prop: on génère le code d'assignation à partir des .ppw de base.

Exemple:
    class Def {
        ²CallFunction  call_function;     ///< optionnal
        Scope         arg_scope;         ///<
        Decl          decl;              ///<
        Ast*          ast;               ///<


