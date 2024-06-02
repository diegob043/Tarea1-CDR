# Cuatro en Línea

Cuatro en Línea es un juego de mesa para dos jugadores distribuido por Hasbro, en el que se introducen fichas en un tablero vertical con el objetivo de alinear cuatro fichas consecutivas de un mismo color. Fue creado en 1974. El objetivo del juego es alinear cuatro fichas sobre un tablero formado por seis filas y siete columnas. Cada jugador dispone de 21 fichas de un color (por lo general, rojas o amarillas). Por turnos, los jugadores deben introducir una ficha en la columna que prefieran (siempre que no esté completa) y esta caerá a la posición más baja. Gana la partida el primero que consiga alinear cuatro fichas consecutivas de un mismo color en horizontal, vertical o diagonal. Si todas las columnas están llenas pero nadie ha hecho una fila válida, hay empate.

Este proyecto implementa el juego Cuatro en Línea mediante el modelo Cliente/Servidor en C++ utilizando sockets.

## Requisitos

- Sistema operativo: Linux (se ha probado en Ubuntu)
- Compilador: g++ (GCC)
- Make (para compilar el proyecto)
- Biblioteca estándar de C++ y sockets

## Instrucciones de Compilación

1. Clona el repositorio o descarga los archivos `servidor.cpp`, `cliente.cpp`, y `Makefile`.
2. Abre una terminal y navega al directorio donde están los archivos.
3. Ejecuta el siguiente comando para compilar ambos programas (servidor y cliente):

    ```bash
    make
    ```
4. Para eliminar los archivos:
1. 
    ```bash
    make clean
    ```
    
## Instrucciones de Ejecución

### Ejecutar el Servidor

1. Abre una terminal y navega al directorio donde compilaste el proyecto.
2. Ejecuta el servidor con el siguiente comando:

    ```bash
    ./servidor <puerto>
    ```

   Donde `<puerto>` es el puerto en el cual quieres que el servidor escuche las conexiones. Por ejemplo:

    ```bash
    ./servidor 7777
    ```

### Ejecutar el Cliente

1. Abre otra terminal y navega al mismo directorio.
2. Ejecuta el cliente con el siguiente comando:

    ```bash
    ./cliente <server_ip> <puerto>
    ```

   Donde `<server_ip>` es la dirección IP del servidor y `<puerto>` es el puerto en el cual el servidor está escuchando. Por ejemplo, si el servidor está ejecutándose en la misma máquina:

    ```bash
    ./cliente 127.0.0.1 7777
    ```

## Cómo Funciona el Juego

### Por Parte del Cliente

1. Conectar al servidor.
2. Visualizar el tablero.
3. Esperar por la indicación del servidor respecto de quién comienza (selección al azar por parte del servidor).
4. Jugar:
   - Indicar columna del tablero donde dejar caer su ficha (de 1 a 7).
   - Esperar y visualizar el resultado.
   - Lo anterior se repite hasta que exista un ganador o un empate.

### Por Parte del Servidor

1. Levantar servicio y esperar por conexiones (clientes).
2. Por cada jugador conectado:
   - Seleccionar al azar quién comienza el juego.
   - Solicitar columna o seleccionar columna al azar según corresponda el turno.
   - Verificar las jugadas.
   - Enviar resultados.
3. Mostrar por la terminal información del estado de las conexiones (conexiones nuevas, cerradas), además de los resultados parciales y finales de los juegos.