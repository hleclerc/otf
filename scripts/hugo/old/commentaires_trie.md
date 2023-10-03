On pourrait stocker une liste d'expression dans le trie pour être capables de le refaire ou d'en faire de nouveaux en présence de nouveaux schémas, mais ça ne marchera pas pour ajouter toutes les nouvelles variables à la volée.

Cependant, si la plupart des variables sont résolues lorsque le code est généré, l'optimisation de mettre les variables dans des entiers ne vaut sans doute pas le coup.

Question : prévoit-on un langage pour pour répet et tout ça ? Ça paraît obligatoire.

Prop: des clés avec toutes les possibilités et les conditions pour y aller
  => au début, on a toutes les possibilités
  => si on rencontre un neof, 
     => si on n'a pas encore spécifié de condition pour eof, on rajoute et on return
     => sinon, onn exclut ce qui ne rentre pas dans la catégorie
  => en passant les join, les neof et les tests de char, 

Pour les chars, une première piste pourrait être de dire "on connait ce char". Par exemple, un entier à -1 par défaut. quand >= 0, c'est un char, à 256, c'est un eof.

Quand on supprime la dernière possibilité, ça veut dire qu'on a fini de manger des caractères pour cette expression et qu'il n'y a pas mieux.

Prop: on pourra éliminer toutes les propositions après celles qui sont sûres d'arriver.

Rq: l'approche la plus propre serait sans doute de réintegrer toutes les possibilités à chaque fois qu'on avance d'un caractère.
  => les unconditionnaly ok élimineront les cas assez vite, mais il faut accompagner avec cette liste pour les prochaines caractères
  => pb: à chaque nouvel ajout, il faudra entièrement refaire le trie.
  => prop: on marque les transitions qui peuvent être influencées par le début.

Rq: on pourrait tolérer un temps long pour refaire les tries si on est capables de mettre en cache.

