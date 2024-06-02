#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <ctime>

class JuegoServidor {
public:
    JuegoServidor(int puerto);
    void ejecutar();

private:
    int socket_servidor;  // Descriptor del socket del servidor
    struct sockaddr_in direccion_servidor;  // Estructura para la dirección del servidor
    std::vector<std::thread> lista_hilos;  // Lista de hilos para manejar clientes
    std::mutex cerradura;  // Mutex para sincronización

    void atenderCliente(int cliente_socket, const std::string& info_cliente);
    void dibujarTablero(char tablero[6][7]);
    bool comprobarGanador(char tablero[6][7], char jugador);
};

JuegoServidor::JuegoServidor(int puerto) {
    // Crea socket del sv
    socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_servidor == 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }
    
    // Configura dirección del sv
    direccion_servidor.sin_family = AF_INET;
    direccion_servidor.sin_addr.s_addr = INADDR_ANY;
    direccion_servidor.sin_port = htons(puerto);

    // Enlaza socket a la dirección
    if (bind(socket_servidor, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor)) < 0) {
        perror("Error al enlazar");
        exit(EXIT_FAILURE);
    }

    // Pone el socket en modo de escucha
    if (listen(socket_servidor, 3) < 0) {
        perror("Error al escuchar");
        exit(EXIT_FAILURE);
    }
}

void JuegoServidor::ejecutar() {
    std::cout << "Servidor en espera de conexiones..." << std::endl;
    srand(time(0));

    while (true) {
        int longitud_direccion = sizeof(direccion_servidor);
        int cliente_socket = accept(socket_servidor, (struct sockaddr *)&direccion_servidor, (socklen_t*)&longitud_direccion);
        if (cliente_socket < 0) {
            perror("Error al aceptar la conexión");
            continue;
        }

        // Obtiene información del cliente
        std::string info_cliente = inet_ntoa(direccion_servidor.sin_addr) + std::string(":") + std::to_string(ntohs(direccion_servidor.sin_port));
        std::cout << "Nuevo juego [" << info_cliente << "]" << std::endl;

        // Crea un nuevo hilo para manejar al cliente
        lista_hilos.push_back(std::thread(&JuegoServidor::atenderCliente, this, cliente_socket, info_cliente));
    }
}

void JuegoServidor::atenderCliente(int cliente_socket, const std::string& info_cliente) {
    char tablero[6][7] = {};
    memset(tablero, '-', sizeof(tablero));

    // Determina al azar quién comienza
    bool turno_cliente = rand() % 2 == 0;
    send(cliente_socket, &turno_cliente, sizeof(turno_cliente), 0);
    std::cout << "Juego [" << info_cliente << "]: " << (turno_cliente ? "El cliente comienza." : "El servidor comienza.") << std::endl;

    while (true) {
        int columna;
        if (turno_cliente) {
            // Recibe movimiento del cliente
            recv(cliente_socket, &columna, sizeof(columna), 0);
            for (int i = 5; i >= 0; --i) {
                if (tablero[i][columna] == '-') {
                    tablero[i][columna] = 'C';
                    break;
                }
            }
            std::cout << "Juego [" << info_cliente << "]: Cliente juega en la columna " << columna + 1 << "." << std::endl;
        } else {
            // Genera movimiento del sv
            columna = rand() % 7;
            for (int i = 5; i >= 0; --i) {
                if (tablero[i][columna] == '-') {
                    tablero[i][columna] = 'S';
                    break;
                }
            }
            std::cout << "Juego [" << info_cliente << "]: Servidor juega en la columna " << columna + 1 << "." << std::endl;
        }

// Envia el estado del tablero al cliente
        send(cliente_socket, tablero, sizeof(tablero), 0);

        // Verifica si hay ganador
        if (comprobarGanador(tablero, turno_cliente ? 'C' : 'S')) {
            char estado = 'G';  // 'G' indica que el jugador actual ha ganado
            send(cliente_socket, &estado, 1, 0);
            std::cout << "Juego [" << info_cliente << "]: " << (turno_cliente ? "El cliente ha ganado." : "El servidor ha ganado.") << std::endl;
            break;
        } else {
            // Verifica si hay empate
            bool empate = true;
            for (int i = 0; i < 7; ++i) {
                if (tablero[0][i] == '-') {
                    empate = false;
                    break;
                }
            }
            if (empate) {
                char estado = 'D';  // 'D' indica empate
                send(cliente_socket, &estado, 1, 0);
                std::cout << "Juego [" << info_cliente << "]: Empate." << std::endl;
                break;
            }
        }

        // Cambia de turno
        char estado = 'C';  // 'C' indica continuar
        send(cliente_socket, &estado, 1, 0);
        turno_cliente = !turno_cliente;
    }

    // Cierra conexión con el cliente
    close(cliente_socket);
    std::cout << "Juego [" << info_cliente << "]: Fin del juego." << std::endl;
}

void JuegoServidor::dibujarTablero(char tablero[6][7]) {
    std::cout << "TABLERO" << std::endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            std::cout << tablero[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------" << std::endl;
    std::cout << "1 2 3 4 5 6 7" << std::endl;
}

bool JuegoServidor::comprobarGanador(char tablero[6][7], char jugador) {
    // Verifica victoria horizontal
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila][col + 1] == jugador &&
                tablero[fila][col + 2] == jugador &&
                tablero[fila][col + 3] == jugador) {
                return true;
            }
        }
    }
    // Verifica victoria vertical
    for (int fila = 0; fila < 3; ++fila) {
        for (int col = 0; col < 7; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col] == jugador &&
                tablero[fila + 2][col] == jugador &&
                tablero[fila + 3][col] == jugador) {
                return true;
            }
        }
    }
    // Verifica victoria diagonal (de abajo a la izquierda a arriba a la derecha)
    for (int fila = 3; fila < 6; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila - 1][col + 1] == jugador &&
                tablero[fila - 2][col + 2] == jugador &&
                tablero[fila - 3][col + 3] == jugador) {
                return true;
            }
        }
    }
    // Verifica victoria diagonal (de arriba a la izquierda a abajo a la derecha)
    for (int fila = 0; fila < 3; ++fila) {
        for (int col = 0; col < 4; ++col) {
            if (tablero[fila][col] == jugador &&
                tablero[fila + 1][col + 1] == jugador &&
                tablero[fila + 2][col + 2] == jugador &&
                tablero[fila + 3][col + 3] == jugador) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: ./servidor <puerto>" << std::endl;
return -1;
    }

    int puerto = std::stoi(argv[1]);
    JuegoServidor servidor(puerto);
    servidor.ejecutar();

    return 0;
}