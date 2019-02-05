#REQUISITOS

1. El usuario debe poder guardar el diseño en el que está trabajando.
2. El usuairo debe poder cargar un diseño existente de un fichero.
3. El usuario debe poder crear un diseño en blanco nuevo.
4. El usuario debe poder pintar rectángulos en el canvas.
5. Los rectángulos que el usuario pinte se unirán formando polígonos.
6. El usuario debe poder mover aristas de los polígonos.
7. El usuario debe poder extraer regiones rectangulares de los polígonos, dibujando dichas regiones a extraer.
8. El usuario debe poder eliminar un vértice siempre y cuando este esé alineado con sus dos vértices vecinos.
9. El usuario debe poder seleccionar un vértice.
10. El usuario debe poder seleccionar un elemento bloqueador.
11. El usuario debe poder seleccionar un elemento llave en la barra lateral. CREATEME
11. El usuario debe poder seleccionar una instancia de un elemento llave.
12. El usuario debe poder añadir un vértice en una arista.
13. El usuario debe poder crear elementos llave en el menú lateral.
14. El usuario debe poder modificar el tipo y especificaciones de un elemento llave en el menú lateral. CREATEME
14. El usuario debe poder instanciar elementos llave en el diseño.
15. El usuario debe poder eliminar elementos llave en el diseño.
16. El usuario debe poder mover elementos llave del diseño.
17. El usuario debe poder eliminar elementos llave en el menú lateral.
18. El usuario debe poder modificar el nombre de un elemento llave en el menú lateral.
19. El usuario debe poder crear elementos bloqueadores bidireccionales.
20. El usuario debe poder crear elementos bloqueadores unidireccionales.
21. El usuario debe poder eliminar cualquier elemento bloqueador del diseño.
22. El usuario debe poder editar las condiciones de apertura de cualquier elemento bloqueador.
23. El usuario debe poder comprobar que su diseño cumple con los requisitos de completabilidad.
24. Se ha de poder transformar un diseño en un grafo de mazmorra.
25. Se ha de notificar el error de completabilidad en caso de que en el diseño del usuario exista un camino que provoque una condición de imcompletabilidad.
26. El usuario debe poder escoger regiones rectangulares de su diseño que conformarán las habitaciones (rooms) del juego.
27. El usuario debe poder editar el nombre de una habitación.
28. El usuario debe poder eliminar una región de habitación.
29. El usuario debe poder extender el tamaño de una habitación moviendo uno de sus lados.
30. No pueden existir dos habitaciones con el mismo nombre.
31. El usuario debe poder convertir las habitaciones de su diseño en un fichero .room compatible con Game Maker Studio 2.
32. La aplicación debe poder ejecutarse en windows.




## El usuario debe poder guardar el diseño en el que está trabajando.
El usuario pulsará un elemento del menú y a continuación se desplegará una ventana donde podrá escoger el nombre del diseño y el directorio en el que quiere guardar el diseño.  
A continuación, se generará un fichero JSON con el nombre especificado y en el directorio escogido. Este fichero debe contener información suficiente para cargar el diseño en la aplicación.  
La extensión del fichero será .maze

## El usuairo debe poder cargar un diseño existente de un fichero.
El usuario pulsará un elemento del menú y a continuación se desplegará en una ventana donde podrá escoger el fichero .maze con el diseño que desea cargar.  
A continuación, la aplicación cargará los datos de dicho fichero, actualizará el canvas para mostrar el diseño cargado y la barra lateral para mostrar las llaves creadas.

## El usuario debe poder crear un diseño en blanco nuevo.
El usuario pulsará un elemento del menú y a continuación la aplicación cargará un diseño vacío sobre el que el usuario podrá trabajar.  
Se actualizará tanto el canvas como la barra lateral para mostrar el estado de un diseño vacío.

## El usuario debe poder pintar rectángulos en el canvas.
El usuario hará click en el canvas para comenzar a pintar un rectángulo por una esquina. A continuación moverá el ratón hasta donde desea que esté la esquina opuesta y soltará el ratón, quedando fijado un rectángulo.  
Durante este proceso de pulsar, mover y soltar, se irá pintando y actualizando el rectángulo. Este solamente quedará fijado en el canvas cuando el usuario termine el click.  

## Los rectángulos que el usuario pinte se unirán formando polígonos.
Se realizará una operación de unión entre el rectángulo creado y el polígono preexistente en el diseño. Al terminar esto, lo vértices en las localizaciones pertinentes (esquinas del rectángulo fuera del interior del polígono preexistente y en las intersecciones de aristas del rectángulo con el polígono) y las aristas que conecten dichos vértices.  
De esta forma, el resultado de añadir un rectángulo al diseño será un polígono.

## El usuario debe poder mover aristas de los polígonos.
Haciendo click en una de las aristas, el usuario podrá arastrarla hasta donde desee. se deberán actualizar las aristas vecinas a la seleccionada.

## El usuario debe poder extraer regiones rectangulares de los polígonos, dibujando dichas regiones a extraer.
El usuario hará click con la tecla shift pulsada para dibujar una región a extraer de la misma manera que dibujaría un nuevo rectángulo. Al terminar de seleccionar dicha región, se extraerá del polígono preexistente en el diseño.

## El usuario debe poder eliminar un vértice siempre y cuando este esé alineado con sus dos vértices vecinos.
Al seleccionar un vértice, el usuario podrá eliminarlo pulsand la tecla supr siempre y cuando se cumpla que las dos aristas adyacentes al vértice estén alineadas

## El usuario debe poder seleccionar un vértice.
El usuario seleccionará un vértice haciendo click izquierdo sobre éste. Al hacer esto, cualquier selección previa se descartará

## El usuario debe poder seleccionar un elemento bloqueador.
El usuario seleccionará un elemento bloqueador del diseño haciendo click izquierdo sobre éste. Al hacer esto, cualquier selección previa se descartará

## El usuario debe poder seleccionar una instancia de un elemento llave.
El usuario seleccionará una instancia de un elemento llave del diseño haciendo click izquierdo sobre ésta. Al hacer esto, cualquier selección previa se descartará

## El usuario debe poder añadir un vértice en una arista.
Al seleccionar una arista, el usuario podrá crear un nuevo vértice con click derecho

## El usuario debe poder crear elementos llave en el menú lateral.
El usuario pulsará un botón de la barra lateral para crear un nuevo elemento llave. Al pulsar este botón, se añadirá un nuevo elemento con un nombre por defecto, que podrá modificar a su gusto.

## El usuario debe poder instanciar elementos llave en el diseño.

## El usuario debe poder eliminar elementos llave en el diseño.

## El usuario debe poder mover elementos llave del diseño.

## El usuario debe poder eliminar elementos llave en el menú lateral.
El usuario seleccionará un elemento llave del menú lateral y, pulsando la tecla supr, este se eliminará de la barra lateral. Además de esto, todas las instancias de dicho elemento en el diseño serán también eliminadas.  
POR ESPECIFICAR: al eliminar un elemento llave, ¿qué hacer con las condiciones de apertura de elementos bloqueadores que hagan referencia al elemento llave eliminado?

## El usuario debe poder modificar el nombre de un elemento llave en el menú lateral.
El usuario seleccionará un elemento llave del menú lateral y, pulsando un botón, podrá cambiarle el nombre

## El usuario debe poder crear elementos bloqueadores bidireccionales.

## El usuario debe poder crear elementos bloqueadores unidireccionales.

## El usuario debe poder eliminar cualquier elemento bloqueador del diseño.

## El usuario debe poder editar las condiciones de apertura de cualquier elemento bloqueador.

## El usuario debe poder comprobar que su diseño cumple con los requisitos de completabilidad.

## Se ha de poder transformar un diseño en un grafo de mazmorra.

## Se ha de notificar el error de completabilidad en caso de que en el diseño del usuario exista un camino que provoque una condición de imcompletabilidad.

## El usuario debe poder escoger regiones rectangulares de su diseño que conformarán las habitaciones (rooms) del juego.

## El usuario debe poder editar el nombre de una habitación.

## El usuario debe poder eliminar una región de habitación.

## El usuario debe poder extender el tamaño de una habitación moviendo uno de sus lados.

## No pueden existir dos habitaciones con el mismo nombre.

## El usuario debe poder convertir las habitaciones de su diseño en un fichero .room compatible con Game Maker Studio 2.

## La aplicación debe poder ejecutarse en windows.