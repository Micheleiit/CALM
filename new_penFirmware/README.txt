Il codice presentato è un firmware per un sistema di controllo che gira su un microcontrollore SAMD21. Il firmware è sviluppato nell'ambiente Arduino IDE e utilizza diverse librerie per gestire la comunicazione con dispositivi esterni e il controllo di un manipolatore laser attraverso un'interfaccia USB-HID con un mouse.
Questo firmware Fornisce un controllo preciso e versatile di un manipolatore laser, sfruttando un mouse per registrare e riprodurre movimenti complessi in tempo reale, con l'invio di comandi tramite un protocollo SPI al dispositivo esterno. La comunicazione avviene tramite SPI. La struttura del codice è suddivisa in diverse sezioni funzionali. Spiegazione dettagliata delle varie parti:

1. Funzionalità dei pulsanti del mouse
Pulsante sinistro:
Pressione breve: Abilita la modalità ZOOM_OUT.
Pressione lungo: Ritorno allo stato NORMAL_ZOOM.

Pulsante centrale:
Pressione breve: Setta la posizione iniziale decisa dall'utilizzatore nel workspace.

Pulsante destro:
Pressione breve: Abilita la modalità ZOOM_IN.
Pressione lungo: Ritorno allo stato NORMAL_ZOOM.

2. Librerie e Definizioni
Include librerie essenziali come MouseController e SPI.

3. Funzioni di Callback del Mouse
MouseMoved: Registra la posizione in base ai movimenti del mouse e la invia al manipolatore.
MousePressed e MouseReleased: Gestiscono gli eventi di pressione e rilascio dei pulsanti del mouse, 
cambiando stato e comportamento a seconda del pulsante e del tempo di pressione.

4. Funzione di REPEART_TRAJECTORY
Versione 1.0: L'idea in questa versione è quella di far si che premendo il pulsante M e trascinando il pennino succedano in contemaporanea due cose:
	1) mandiamo i dati ad RM in real time per far muovere il laser
	2) storiamo i dati degli spostamenti (x,y) in un vettore SPOSTAMENTI
Nel momento in cui viene rilasciato il pulsante M allora viene mandato un messaggio SPI ad RM affinchè venga ripetuta la traiettoria precedentemente eseguita.
Affinchè questo sia possibile, nel momento in cui il pulsante M viene rilasciato i dati sulle posizioni (x,y) mandati ad RM tramite SPI saranno quelli salvati nel vettore SPOSTAMENTI.
Per interrompere la funzionalià di REPEAT_TRAJECTORY bisognerà premere rapidamente il pulsante M. (?)

