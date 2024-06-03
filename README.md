Integrantes:

Diego Ampuero

Felipe Cifuentes

# Cuatro en Línea

"Cuatro en Línea" es un juego de mesa distribuido por Hasbro para dos jugadores, ideado en 1974. Su dinámica es simple pero estratégica: los jugadores colocan fichas de su color en un tablero vertical de seis filas por siete columnas, con el objetivo de ser el primero en alinear cuatro fichas del mismo color en horizontal, vertical o diagonal. Cada jugador tiene 21 fichas de un color, usualmente rojas o amarillas. Por turnos, los jugadores colocan una ficha en la columna que elijan (siempre que no esté llena), y esta cae hasta la posición más baja disponible en esa columna. Si todas las columnas están llenas pero ningún jugador ha logrado una alineación, la partida termina en empate. Es un juego que combina estrategia, anticipación y habilidad para bloquear al oponente mientras se busca la propia victoria.

Proyecto implementa el juego Cuatro en Línea mediante el modelo Cliente/Servidor en C++ utilizando sockets.

## Requisitos/Requerimientos

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