#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <wiringPi.h> // Biblioteca para manejar pines GPIO en Raspberry Pi

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime[] = {100, 100, 100, 100}; // Retardo en milisegundos

void choque();
void pinSetup(void);
void ledShow(unsigned char);
int delay(int index);
void turnOff();
bool keyHit(int index);

int main(void) {
    wiringPiSetup(); // Inicialización de wiringPi para los pines GPIO

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
    // No hay función _kbhit() en Linux, podemos hacer una pequeña pausa para verificar la entrada
    delay(0); // Pausa breve para evitar consumo excesivo de CPU

    if (digitalRead(27) == LOW) { // GPIO 27 conectado a ESC (ajusta según tu conexión)
        return true;
    }

    if (digitalRead(17) == LOW) { // GPIO 17 conectado a W (ajusta según tu conexión)
        if (delayTime[index] > 10) {
            delayTime[index] -= 10;
        }
    }

    if (digitalRead(22) == LOW) { // GPIO 22 conectado a S (ajusta según tu conexión)
        delayTime[index] += 10;
    }

    return false;
}

void pinSetup(void) {
    for (int i = 0; i < 8; i++) {
        pinMode(led[i], OUTPUT);
    }

    pinMode(27, INPUT); // GPIO 27 configurado como entrada para ESC
    pinMode(17, INPUT); // GPIO 17 configurado como entrada para W
    pinMode(22, INPUT); // GPIO 22 configurado como entrada para S
}

void ledShow(unsigned char output) {
    for (int j = 0; j < 8; j++) {
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
