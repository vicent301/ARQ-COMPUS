#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h> // Biblioteca para Windows que reemplaza algunas funciones de termios
#include <time.h> // Para usar clock()

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime[] = {100, 100, 100, 100}; // Retardo en milisegundos

void choque();
void pinSetup(void);
void ledShow(unsigned char);
int delay(int index);
void turnOff();
bool keyHit(int index);

int main(void) {
    pinSetup();
    choque();
    return 0;
}

void choque() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione W para aumentar la velocidad\n");
    printf("Presione S para disminuir la velocidad\n");
    printf("El Choque:\n");

    unsigned char output, aux1, aux2;

    while (true) {
        aux1 = 0x80;
        aux2 = 0x1;
        for (int i = 0; i < 7; i++) {
            output = aux1 | aux2;
            ledShow(output);
            aux1 = aux1 >> 1;
            aux2 = aux2 << 1;

            if (delay(1) == 0) {
                turnOff();
                return;
            }
        }
    }
}

bool keyHit(int index) {
    if (_kbhit()) {
        int ch = _getch();

        if (ch == 119) { // Tecla W
            if (delayTime[index] > 10) {
                delayTime[index] = delayTime[index] - 10;
            }
        }

        if (ch == 115) { // Tecla S
            delayTime[index] = delayTime[index] + 10;
        }

        if (ch == 27) { // Tecla ESC
            return true;
        }
    }
    return false;
}

void pinSetup(void) {
    // Configuración inicial de pines, si es necesario
}

void ledShow(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        // Aquí deberías reemplazarlo con la función que maneje los LEDs en tu plataforma
        if ((output >> j) & 1) {
            printf("*"); // Imprimir asterisco si el bit está activo (1)
        } else {
            printf(" "); // Imprimir espacio si el bit está inactivo (0)
        }
    }
    printf("\n");
}

int delay(int index) {
    clock_t start_time = clock();
    clock_t wait_time = delayTime[index] * CLOCKS_PER_SEC / 1000; // Convertir a unidades de reloj

    while ((clock() - start_time) < wait_time) {
        if (keyHit(index)) {
            return 0;
        }
    }

    return 1;
}

void turnOff() {
    unsigned char off = 0x0;
    ledShow(off);
}
