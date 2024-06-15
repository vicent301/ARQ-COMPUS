#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "EasyPIO.h"

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
long delayTime = 10000000; // Velocidad inicial de la secuencia en nanosegundos

void pinSetup(void) {
    pioInit();

    for (int i = 0; i < 8; i++) {
        pinMode(led[i], OUTPUT);
    }
}

void ledShow(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        digitalWrite(led[j], (output >> j) & 1);
    }
}

void delayNanoseconds(long nanoseconds) {
    struct timespec req = {0};
    req.tv_sec = 0;
    req.tv_nsec = nanoseconds;

    // Espera el tiempo especificado en nanosegundos
    while (nanosleep(&req, &req) && errno == EINTR);
}

int delay(void) {
    delayNanoseconds(delayTime);
    return 1; // Retorno exitoso del retardo
}

void turnOff(void) {
    unsigned char off = 0x0;
    ledShow(off);
}

void autoFantastico(void) {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione W para aumentar la velocidad\n");
    printf("Presione S para disminuir la velocidad\n");
    printf("Auto Fantastico:\n");

    unsigned char output;

    while (true) {
        output = 0x80;
        for (int i = 0; i < 8; i++) {
            ledShow(output);
            output = output >> 1;

            if (!delay()) {
                turnOff();
                return;
            }
        }

        output = 0x02;
        for (int i = 0; i < 6; i++) {
            ledShow(output);
            output = output << 1;

            if (!delay()) {
                turnOff();
                return;
            }
        }
    }
}

int main(void) {
    pinSetup();
    autoFantastico();
    return 0;
}
