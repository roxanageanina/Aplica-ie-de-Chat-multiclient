# Aplicatie-de-Chat-multiclient

Surse de documentare și informare: 

  https://www.youtube.com/watch?v=dquxuXeZXgo&t=1382s
  https://www.youtube.com/watch?v=yUopCbQYpUI
  https://www.youtube.com/watch?v=6YclNlhVVb0&t=60s
  https://stackoverflow.com/questions/25542887/multithreading-in-client-server-chat-app-in-c-linux
  https://wiki.qt.io/WIP-How_to_create_a_simple_chat_application
  https://ome.tv/

Introducere:
  Aplicația de chat dezvoltată permite utilizatorilor să se conecteze și să converseze cu persoane din diverse părți ale lumii, oferind o experiență similară cu cea oferită de platforma Omegle. Utilizatorii pot trimite mesaje text și imagini, având posibilitatea de a evalua aplicația înainte de închiderea acesteia.

Scopul aplicației:
  Scopul principal al aplicației este de a facilita întâlnirea și interacțiunea între persoane care doresc să cunoască alți utilizatori anonimi din diferite culturi și țări.

Funcționalități principale:
1. Conectarea și deconectarea utilizatorilor:
  - utilizatorii pot începe și opri sesiunile de chat.
  - este necesară selectarea genului și a țării pentru a putea fi conectați la un partener de conversație.
2. Trimiterea și primirea de mesaje text:
  - utilizatorii pot trimite mesaje text către partenerii lor de chat.
  - mesajele primite sunt afișate în fereastra de chat.
3. Trimiterea și primirea de imagini:
  - utilizatorii pot trimite imagini partenerilor lor.
  - imaginile primite pot fi vizualizate într-o fereastră separată, la cererea utilizatorului.
4. Evaluarea aplicației:
  - înainte de închiderea aplicației, utilizatorii sunt întrebați să ofere o notă de evaluare (între 1 și 10).
  - Notele sunt înregistrate într-un fișier text pentru a monitoriza feedback-ul utilizatorilor.
5. Interfață grafică personalizată:
  - interfața grafică utilizează o combinație de culori turcoaz și alb.
  - textul este afișat în negru pentru o mai bună vizibilitate.

Componente ale aplicației:
1. Clientul (ClientWindow)
   - gestionează conexiunea la server și trimiterea informațiilor despre utilizator (gen și țară).
   - oferă opțiuni pentru trimiterea de mesaje text și imagini.
   - afișează mesajele și imaginile primite de la partenerii de chat.
   - permite evaluarea aplicației înainte de închiderea acesteia.
3. Serverul (ServerWindow)
  - ascultă și acceptă conexiuni de la clienți.
  - gestionează crearea de sesiuni de chat între clienți conectați.
  - monitorizează și înregistrează conexiunile și deconexiunile clienților.
    
Implementare tehnică:
  - limbaj de programare: C++
  - framework: Qt
  - comunicare: Comunicarea între client și server se realizează folosind socket-uri TCP pentru transmiterea datelor.
