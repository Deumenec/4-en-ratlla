10/10/2025

Per a l'algorisme MinMax per decidir quina és la millor jugada, assignarem inicialment unes normes molt senzilles: si una jugada (camí) ens fa guanyar, aquesta tindrà valor 1 però per sobre d'això imposem que el nostre oponent no pugui guanyar-nos. així doncs classifiquem cada node inicialment amb puntuacions 0 si el nostre oponent guanya, 0.5 si no guanya ni ell ni nosaltres i 1 si guanyem nosaltres.

Per altra banda, observem que per com es construeix l'arbre, si la partida es guanya/perd en aquell torn aleshores es segueix construint, això està malament però l'algorisme està adaptat perqué no importi a l'hora de decidir les jugades... mentida, això ho he d'arreglar!


