# algoritmo utilizzando i set
Scrivo l'algoritmo utilizzando la struttura dati set per avere un porting il piu' fedele possibile, il generator in Python utilizza i set e le liste.
Questo per fare delle comparazioni e poi passare ad implementazioni piu' efficienti.

# set
tipica implementazione interna: binary search trees

Qui il problema e' che questa struttura dati in c++ e' ordinata, non e' possibile applicare lo shuffle direttamente su di essa. (file err_set_shuffle.txt) per vedere l'errore)

idee soluzione:
- estrazione casuale (attenzione distribuzione uniforme)
- usare un'altra struttura dati di supporto

pro:
- inizializzazione efficiente
- operazione differenza tra set efficiente

# unordered set
I set in Python sono unordered.


# NOTE:
https://stackoverflow.com/questions/12761315/random-element-from-unordered-set-in-o1/31522686#31522686

random access iterator: non disponibile per i set, non e' possibile utilizzare std::shuffle
                      : non e' possibile accedere direttamente ad un elemento, (linked list)

L'implementazione come quella in python sembrerebbe essere troppo pesante
