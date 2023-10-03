Les ref_count ne fonctionnant pas pour les graphs, on est tentés d'ajouter un stockage des parents pour trouver si on peut libérer un objet
  On parcourt les parents pour voir si on trouve un ref_count > 0
  Les ref_count servent à savoir si l'objet y est référencé et sont modifiés par l'interpreteur mais potentiellement dans des embranchements

Pb de la mort: travailler sur graph + une liste de parents => pas thread safe.


