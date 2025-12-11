*Laberinto Aleatorio con Búsqueda de Caminos*

*Descripción del Proyecto*

Este proyecto es un generador de laberintos aleatorios cuadrados e impares, implementado en lenguaje C. El objetivo es crear un laberinto aleatorio de un tamaño determinado y ofrecer la opción de resolverlo utilizando algoritmos de búsqueda de caminos: BFS (Breadth-First Search) y DFS (Depth-First Search).

*Características*

- Generación de laberintos aleatorios cuadrados e impares de cualquier tamaño
- Implementación de algoritmos de búsqueda de caminos:
- BFS (Breadth-First Search): búsqueda en anchura
- DFS (Depth-First Search): búsqueda en profundidad
- Visualización del proceso de búsqueda y resolución del laberinto

*Algoritmos Implementados*

BFS (Breadth-First Search)
El algoritmo BFS es un método de búsqueda en anchura que explora todos los nodos a una distancia determinada antes de avanzar a la siguiente distancia. En este proyecto, se utiliza BFS para encontrar el camino más corto desde el punto de inicio hasta el punto de fin del laberinto.

DFS (Depth-First Search)
El algoritmo DFS es un método de búsqueda en profundidad que explora lo más lejos posible a lo largo de cada rama antes de retroceder. En este proyecto, se utiliza DFS para encontrar un camino desde el punto de inicio hasta el punto de fin del laberinto, aunque no necesariamente el más corto.

Backtracking
El algoritmo de backtracking se utiliza para generar el laberinto aleatorio. Funciona creando un camino aleatorio y retrocediendo cuando se encuentra un callejón sin salida, lo que permite crear un laberinto complejo y aleatorio.

*Uso del Proyecto*

1. Clona el repositorio y compila el código con `gcc`
2. Ejecuta el programa y sigue las instrucciones para generar un laberinto aleatorio
3. Selecciona el algoritmo de búsqueda de caminos deseado (BFS o DFS)
4. Observa cómo se resuelve el laberinto y se muestra el camino encontrado

