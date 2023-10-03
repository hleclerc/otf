Proxies
=======

Attention: quand on teste un proxy, quand on demande un attribut, il faut le faire sur la valeur de-proxifiée, 
  mais en argument de fonction, il faut bien passer la ref

def pfunc f, args, num
    f args... # prop: les arguments de fonction contiennent un flag `no_proxy_test`.

a := proxy pfunc
print a # => 
a.smurf

La proposition, c'est de faire la sélection de la fonction à appeler. À ce moment là, on a tous les arguments dans un scope. S'il 
