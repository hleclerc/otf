Codegen
=======

On pourrait faire type_hint dans tous les inputs et outputs de toutes les instructions.

Prochains objectifs:
* faire des ifs
* faire des boucles

Pb avec les ifs: en l'état, on est obligés de faire une sauvegarde du scope.

La proposition pourrait être 
* de ne pas passer par des arguments (on sort du cadre des fonctions avec arguments "spéciaux")
* ou de faire des cas particuliers pour certains types d'arguments