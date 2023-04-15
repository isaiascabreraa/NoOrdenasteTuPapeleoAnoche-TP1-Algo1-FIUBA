# Juego- NoOrdenasteTuPapeleoAnoche

1. Introducción
Luego de todos los problemas que tuviste para llegar a FIUBA, finalmente pudiste rendir el temible examen. Ahora
llegó el momento de firmar la libreta, pero te diste cuenta que el día que está el profesor vos no podes ir. Te acordás
que un amigo tuyo también tiene que ir, y le pedís si te hace el favor de llevar tu libreta a firmar.
Tu amigo, Mike Wazowski, acepta el pedido, pero como buen interesado que es, lo hace a cambio de un obsequio.
Al llegar al Departamento de Computación, Mike se encuentra con Roz, quien, una vez más, le recrimina que ”no
ordenó su papeleo anoche...”. Y en venganza, repartió todos los papeles por distintas habitaciones para que se le
haga más difícil a Mike ordenarlos. Y, lamentablemente, Roz no le va a firmar la libreta hasta que él junte todo su
papeleo.
Sin posibilidad de volver a evadir a Roz, llegó el momento de que Mike junte su papeleo, y nosotros vamos a
acompañarlo en esta aventura.

2. Objetivo
El presente trabajo práctico tiene como objetivo evaluar a los alumnos en aspectos fundamentales de la programación. Estos aspectos son:
Validación de datos ingresados por el usuario.
Diseño y desarrollo de funcionalidades de una biblioteca con un contrato preestablecido.
El correcto uso de estructuras de control.
Tipos de dato simples y estructurados.
Buenas prácticas de programación.
Modularización.

3. Enunciado
Como desarrolladores de este juego, debemos ayudar a Mike a juntar todo su papeleo y llevárselo a Roz para que
finalmente tu libreta sea firmada.
El juego consiste en pasar por una serie de raras habitaciones, para que Mike junte el papeleo en orden, que estará
disperso en ellas.
Habrá 3 habitaciones donde buscar. Dichas habitaciones, estarán delimitadas por los bordes del terreno, y además
habrá paredes, obstáculos y herramientas para ayudarte, o perjudicarte, en tu búsqueda.
Mike tendrá distinta cantidad de movimientos iniciales disponibles en cada nivel, y los movimientos que le sobren
del nivel anterior, se sumarán a los iniciales del próximo nivel.
Pero cuidado! La habitación va a rotar con tus movimientos.
En esta segunda etapa de desarrollo, se deberá implementar la lógica del funcionamiento total del juego.

3.1. Gravedad
El jugador podrá caminar horizontalmente sobre paredes, en caso de moverse hacia una coordenada en la cual no
haya una pared debajo, este deberá caer hasta chocar con alguna.

3.2. Acciones del jugador
El jugador podrá moverse, rotar el mapa, utilizar martillos o extintores.
A continuación se detallan las acciones y los caracteres que se deben ingresar por cada una de ellas:

3.2.1. Movimientos laterales
El jugador podrá moverse lateralmente de a una coordenada a la vez.
Movimiento a izquierda: ’A’.
Movimiento a derecha: ’D’.
Por cada movimiento lateral logrado se deberá restar un movimiento al jugador.

3.2.2. Movimientos rotacionales
El jugador podrá rotar el mapa en sentido horario o antihorario.
Rotación horaria: ’E’.
Rotación antihoraria: ’Q’.
Por cada movimiento rotacional se deberá restar un movimiento al jugador.

3.2.3. Martillos
El jugador podrá utilizar un martillo de su mochila en alguna pared a la cual se encuentre adyacente no diagonal
y esta no forme parte del borde del mapa.
Para indicar esto, el usuario tiene que ingresar el siguiente caracter:
Utilizar martillo: ’Z’.
Si el jugador tiene martillos suficientes, se deberá preguntar al usuario la posición respecto al jugador en la que
se quiere martillar, es decir arriba del jugador (ingresa caracter ’W’), abajo (’S’), a su derecha (’D’) o a su izquierda
(’A’).
Si hay una pared (no perteneciente al borde) en la posición indicada, se debe borrar la pared del vector de paredes
y restar un martillo.

3.2.4. Extintores
El jugador podrá utilizar un extintor de su mochila en algún fuego al cual se encuentre adyacente a su izquierda,
adyacente a su derecha o adyacente arriba.
Para indicar esto, el usuario tiene que ingresar el siguiente caracter:
Utilizar extintor: ’C’.
Si el jugador tiene extintores suficientes, se deberá preguntar al usuario la posición respecto al j en jugador a que
se quiere extinguir el fuego, es decir arriba del jugador (ingresa caracter ’W’), a su derecha (’D’) o a su izquierda
(’A’).
Si hay un fuego en la posición indicada, se debe borrar el fuego del vector de obstáculos y restar un extintor.

3.3. Interacción Jugador-Objeto
A continuación se listarán las distintas interacciones que tendrá el jugador con los demás objetos del juego. Es
importante aclarar que el jugador ’choca’ con otro elemento cuando se mueve a su posición o pasa sobre la misma.

3.3.1. Fuegos
Si el jugador choca con un fuego pierde todos sus movimientos, es decir pierde el juego.

3.3.2. Medias
Si el jugador choca con una media, pierde 10 movimientos.

3.3.3. Interruptor ’ahuyenta Randall’
Si el jugador choca con el interruptor lo activa si este se encuentra desactivado o lo desactiva en caso contrario.

3.3.4. Botellas de gritos
Si el jugador choca con una botella de gritos, gana 7 movimientos.

3.3.5. Papeleos
Si el jugador choca con un papeleo, podrá recolectarlo si y solo si los papeleos con ’id_papeleo’ menor a este
fueron recolectados.
Si esto se cumple, se debe borrar lógicamente el papeleo del vector de papeleos utilizando el campo ’recolectado’.
Observación: Las medias, interruptores y botellas de gritos no se eliminan del vector una vez que el jugador se
choca con algunos de estos. En el caso de los fuegos no se tendrá que eliminar (a menos que se los apague con un
extintor), ya que terminará el juego si el jugador se choca con un fuego,

3.4. Nuevos Obstáculos
Se introducirán acciones a medida que el jugador va utilizando movimientos (laterales o rotaciones) que representarán nuevos obstáculos:

3.4.1. Randall
Si el interruptor ’ahuyenta Randall’ no se encuentra activado, un papeleo (no recolectado) aleatorio se transportará
a otra posición aleatoria, válida y libre si la cantidad de movimientos realizados por el jugador es divisible por R,
siendo R un valor distinto por nivel. Decimos que esto lo hace nuestro enemigo Randall quien, cómo todo camaleón,
tiene la habilidad de camuflarse para no ser percibido.
Valor divisible de R por nivel
Nivel 1: 7
Nivel 2: 5
Nivel 3: 3
3.4.2. Nuevas paredes random
Durante los primeros X movimientos del jugador en el respectivo nivel, aparecerá una nueva pared en una posición
aleatoria, válida y libre.
Cantidad de paredes random por nivel
Nivel 1: 40
Nivel 2: 30
Nivel 3: 20
3.5. Objetivo del juego
El juego consiste en atravesar tres niveles, donde cada uno de estos se gana si se recolectan los papeleos en el
orden establecido.
El jugador acumulará solamente los movimientos ahorrados de nivel en nivel.

4. Especificaciones
Para poder lograr ayudar a Mike con su papeleo, se te pedirá implementar nuevas funciones y procedimientos.

4.1. Convenciones
Se deberá utilizar la siguiente convención para los elementos que tengan una coordenada asociada como obstáculos,
algunas herramientas y para el jugador, y para el ingreso de acciones por el usuario:
4.3.1. Convención para identificadores de elementos
Fuegos: F.
Medias: M.
Botellas de gritos: G.
Interruptores ’ahuyenta Randall’: I.
Mike: W.

4.1.2. Convención para ingreso de acción
Utilizar martillo: Z.
Utilizar extintor: C.
Mover o martillar/extinguir a izquierda: A.
Mover o martillar/extinguir a derecha: D.
Martillar/extinguir hacia arriba: W.
Martillar hacia abajo: S.
Mov. rotacional horario: E.
Mov. rotacional antihorario: Q.

5. Resultado esperado
Se espera que el trabajo creado cumpla con las buenas prácticas de programación y todas las funciones y procedimientos pedidos funcionen acorde a lo solicitado, respetando las pre y post condiciones propuestas.

6. Compilación y Entrega
El trabajo práctico debe ser realizado en un archivo llamado papeleo.c, lo que sería la implementación de la
biblioteca papeleo.h. Además, deberán crear una bilioteca propia con lo resuelto en el trabajo práctico 1, y deberá
realizarse el cuestionario previo al juego. El trabajo debe ser compilado sin errores al correr desde la terminal el
comando:
 gcc *.c utiles.o -o juego -std=c99 -Wall -Wconversion -Werror -lm

Aclaración: El main tiene que estar desarrollado en un archivo juego.c, el cual manejará todo el flujo del programa.
utiles.o es un archivo compilado realizado por la cátedra, que pondrá a su disposición funciones que son necesarias
para el armado del juego. Ver anexo.
Lo que nos permite *.c es agarrar todos los archivos que tengan extensión .c en el directorio actual y los compile
todos juntos. Esto permite que se puedan crear bibliotecas a criterio del alumno, aparte de la exigida por la cátedra.
Por último, debe ser entregado en la plataforma de corrección de trabajos prácticos Chanutron2021 (patente
pendiente), en la cual deberá tener la etiqueta ¡Exito! significando que ha pasado las pruebas a las que la cátedra
someterá al trabajo.

