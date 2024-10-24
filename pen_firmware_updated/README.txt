					STRUTTURE STATICHE vs STRUTTURE DINAMICHE IN C

La differenza principale tra strutture statiche e dinamiche riguarda il modo in cui la memoria viene allocata e gestita, e quindi le loro caratteristiche in termini di flessibilità, utilizzo delle risorse, e gestione della memoria.

Ecco una spiegazione dettagliata delle differenze.

1. STRUTTURE STATICHE

Le strutture statiche sono allocate con una dimensione fissa e la memoria viene assegnata al momento della compilazione. Ciò significa che la dimensione della struttura è determinata in fase di scrittura del codice e non può essere modificata durante l'esecuzione del programma.

Caratteristiche delle strutture statiche:
Allocazione statica: La memoria viene allocata automaticamente dal sistema quando il programma viene compilato o caricato. La memoria è solitamente allocata nello stack o nei segmenti globali.
Dimensione fissa: La dimensione della struttura è nota in fase di compilazione, quindi non può essere cambiata in fase di esecuzione.
Velocità: L'accesso a strutture statiche è molto veloce, perché la loro memoria è già allocata e non richiede chiamate come malloc() o free().
Gestione automatica della memoria: Non è necessario gestire manualmente l'allocazione e la deallocazione della memoria, il che riduce il rischio di errori come memory leaks o dangling pointers.

Esempio:
#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];  // Array di dimensione fissa
    int count;           // Numero di elementi utilizzati
} StaticArray;

StaticArray myArray;  // Allocazione statica

In questo esempio, l'array data all'interno della struttura ha una dimensione fissa (MAX_SIZE), che è nota al momento della compilazione. Non puoi ridimensionare questo array durante l'esecuzione del programma.

Svantaggi delle strutture statiche:
Flessibilità limitata: La dimensione della struttura è fissa e non può essere cambiata in fase di esecuzione. Se hai bisogno di più memoria rispetto a quella allocata, non puoi espandere la struttura.
Utilizzo inefficiente della memoria: Se la struttura allocata è troppo grande rispetto alle necessità, si può sprecare memoria. Se è troppo piccola, si rischia di non poter memorizzare tutti i dati necessari.

2. STRUTTURE DINAMICHE

Le strutture dinamiche sono allocate in modo flessibile durante l'esecuzione del programma. La memoria viene allocata manualmente usando funzioni come malloc() o new, e deve essere deallocata esplicitamente con free() o delete quando non è più necessaria.

Caratteristiche delle strutture dinamiche:
Allocazione dinamica: La memoria viene allocata durante l'esecuzione del programma, quindi la dimensione della struttura può variare a seconda delle necessità.
Flessibilità: Le strutture dinamiche possono essere ridimensionate o liberate in qualsiasi momento, il che le rende ideali per gestire quantità variabili di dati.
Gestione manuale della memoria: È necessario allocare esplicitamente la memoria (ad esempio, con malloc() in C o new in C++) e rilasciarla quando non è più necessaria (con free() o delete). Questo introduce il rischio di memory leaks (perdita di memoria) se la memoria non viene deallocata correttamente.

SOLUZIONE ADOTATTA PER LA GESTIONE DELLE TRAIETTORIE

La struttura trajectory_calm è una struttura dinamica. Questo è perché la memoria per la struttura viene allocata a runtime utilizzando la funzione malloc(). La struttura stessa ha alcuni elementi statici, come gli array con dimensione fissa (pos_x_double, pos_y_double e index_values), ma la memoria per l'intera struttura viene comunque allocata dinamicamente.
Questi array fanno parte della struttura e la loro dimensione non può essere modificata a runtime, quindi la loro dimensione è statica.
Tuttavia, la struttura nel complesso non è considerata "statica", poiché l'intera struttura viene allocata dinamicamente in memoria con malloc().
La combinazione di questi due elementi (array statici all'interno della struttura e allocazione dinamica della struttura stessa) significa che la struttura trajectory_calm è dinamica, poiché la sua memoria viene allocata dinamicamente e può essere deallocata esplicitamente, quando non è più necessaria, usando il comando free().
Se prevedi di riutilizzare la funzione init_trajectory_struct() più volte (ad esempio, registrando nuove traiettorie senza riavviare il sistema), liberare la memoria precedente con free() diventa importante per evitare di esaurire la RAM del sistema! Quando decidi di iniziare una nuova traiettoria, devi riallocare la memoria usando di nuovo init_trajectory_struct().
L'uso di array statici all'interno di una struttura dinamica rende più facile gestire la memoria, poiché non devi occuparti di allocare e liberare singolarmente ogni array. Allocando dinamicamente l'intera struttura, gli array statici all'interno vengono automaticamente gestiti insieme a essa.

Vantaggio: Puoi liberare l'intera struttura (compresi gli array statici) con una sola chiamata a free(), semplificando la gestione della memoria.
