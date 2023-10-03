Objectifs pour le parser
========================
    * ça serait bien si on pouvait parser en une passe...
      mais pour savoir à quelles variables on fait référence, il faut l'integralité du scope
      ou alors, on ne fait pas de distinction entre les variables globales et les variables catchées
        et du coup, les variables catchées sont stockées par nom
    * Rq: il faudrait vérifier si une variable peut être trouvée à la fois en arrière et en avant
      on doit pouvoir le faire de façon retrospective, par exemple au moment où on déclare les variables, regarder si les captures étaient censées utilisé des variables du passé

    * idéalement, il faudrait écrire directement dans les sections binaires
       Quel format adopter ?
       
       
Pb: prendre en compte les scope à exécution différés et les scopes à execution immédiate
  => dans les fonctions à déclaration, on peut avoir du différé ou pas (`:=` vs `def`)
  