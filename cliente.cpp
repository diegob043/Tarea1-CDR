#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PUERTO 7777

class JuegoCliente {
public:
    JuegoCliente(const char* ip, int port);
    void ejecutar();

private:
    int socket_fd;
    struct sockaddr_in servidor_addr;

    void conectarServidor(const char* ip, int port);
    void gestionarJuego();
    void imprimirTablero(const char tablero[6][7]);
};

JuegoCliente::JuegoCliente(const char* ip, int port) {
    conectarServidor(ip, port);
}

void JuegoCliente::conectarServidor(const char* ip, int port) {
    // Crea el socket
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Error al crear el socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Configura dirección del sv
    servidor_addr.sin_family = AF_INET;
    servidor_addr.sin_port = htons(port);

    // Convierte dirección IP
    if (inet_pton(AF_INET, ip, &servidor_addr.sin_addr) <= 0) {
        std::cerr << "Dirección IP no válida o no soportada" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Conecta al sv
    if (connect(socket_fd, (struct sockaddr*)&servidor_addr, sizeof(servidor_addr)) < 0) {
        std::cerr << "Error al conectar con el servidor" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void JuegoCliente::ejecutar() {
    gestionarJuego();
}

void JuegoCliente::gestionarJuego() {
    char tablero[6][7];
    memset(tablero, '-', sizeof(tablero)); // Inicializa el tablero

    bool turnoCliente;
    recv(socket_fd, &turnoCliente, sizeof(turnoCliente), 0); // Recibe quién empieza

    while (true) {
        imprimirTablero(tablero);
        if (turnoCliente) {
            int columna;
            std::cout << "Es tu turno. Introduce una columna (1-7): ";
            std::cin >> columna;
            columna--;
            send(socket_fd, &columna, sizeof(columna), 0); // Envia columna elegida
        }

        recv(socket_fd, tablero, sizeof(tablero), 0); // Recibe tablero actualizado
        imprimirTablero(tablero);

        char estado;
        recv(socket_fd, &estado, 1, 0); // Recibe estado del juego
        if (estado == 'G') {
            std::cout << (turnoCliente ? "¡Has ganado!" : "El servidor ha ganado") << std::endl;
            break;
        } else if (estado == 'D') {
            std::cout << "Es un empate." << std::endl;
            break;
        }

        turnoCliente = !turnoCliente; // Cambia turno
    }

    close(socket_fd);
}

void JuegoCliente::imprimirTablero(const char tablero[6][7]) {
    std::cout << "TABLERO DE JUEGO" << std::endl;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            std::cout << tablero[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-----------------" << std::endl;
    std::cout << "1 2 3 4 5 6 7" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: ./cliente <ip_servidor> <puerto>" << std::endl;
        return -1;
    }

    // Crea una instancia del cliente del juego y empieza a jugar
    JuegoCliente cliente(argv[1], atoi(argv[2]));
    cliente.ejecutar();
    return 0;
}