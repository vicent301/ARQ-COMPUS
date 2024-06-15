#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ncurses.h>
#include "EasyPIO.h"

#define PASSWORD_LENGTH 5

void disp_binary(int);
void getPassword(char *password);
void menu();
void autoFantastico();
void choque();
struct termios modifyTerminalConfig(void);
void restoreTerminalConfig(struct termios);
bool keyHit(int index);
void pinSetup(void);
void ledShow(unsigned char);
int delay(int index);
void clearInputBuffer();
void turnOff();

const unsigned char led[] = {14, 15, 18, 23, 24, 25, 8, 7};
int delayTime[] = {10000, 10000, 10000, 10000};

int main(void) {
    pinSetup();
    char setPassword[5] = {'h', 'e', 'l', 'l', 'o'};
    char passwordInput[5];

    for (int i = 0; i < 3; i++) {
        bool passwordFlag = true;
        getPassword(passwordInput);

        for (int j = 0; j < 5; j++) {
            if (setPassword[j] != passwordInput[j]) {
                passwordFlag = false;
                break;
            }
        }

        if (passwordFlag) {
            printf("Bienvenido al sistema !\n\n");
            menu();
            printf("Hasta luego !\n");
            break;
        } else {
            printf("Clave incorrecta\n\n");
        }
    }
}

void disp_binary(int i) {
    int t;
    for (t = 128; t > 0; t = t / 2)
        if (i & t) printf("1 ");
        else printf("0 ");
    fflush(stdout);
    printf("\r");
}

void getPassword(char *password) {
    struct termios oldattr = modifyTerminalConfig();

    printf("Ingrese su clave: ");

    for (int i = 0; i < PASSWORD_LENGTH; i++) {
        password[i] = getchar();
        printf("*");
        fflush(stdout);
    }

    restoreTerminalConfig(oldattr);
    printf("\n");
}

void menu() {
    int opcion;

    do {
        clearInputBuffer();
        printf("Seleccione una opcion:\n");
        printf("1: Auto Fantastico\n");
        printf("2: El Choque\n");
        printf("0: Salir\n");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                autoFantastico();
                break;
            case 2:
                choque();
                break;
            case 0:
                break;
            default:
                printf("Seleccione una opcion valida\n");
        }
    } while (opcion != 0);
}

void autoFantastico() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione W para aumentar la velocidad\n");
    printf("Presione S para disminuir la velocidad\n");
    printf("Auto Fantastico:\n");

    unsigned char output;

    while (true) {
        output = 0x80;
        for (int i = 0; i < 8; i++) {
            ledShow(output);
            disp_binary(output);
            output = output >> 1;

            if (delay(0) == 0) {
                turnOff();
                return;
            }
        }
        output = 0x2;

        for (int i = 0; i < 6; i++) {
            ledShow(output);
            disp_binary(output);
            output = output << 1;

            if (delay(0) == 0) {
                turnOff();
                return;
            }
        }
    }
}

void choque() {
    printf("Presione esc para finalizar la secuencia\n");
    printf("Presione W para aumentar la velocidad\n");
    printf("Presione S para disminuir la velocidad\n");
    printf("Choque:\n");

    unsigned char output, aux1, aux2;

    while (true) {
        aux1 = 0x80;
        aux2 = 0x1;
        for (int i = 0; i < 7; i++) {
            output = aux1 | aux2;
            ledShow(output);
            disp_binary(output);
            aux1 = aux1 >> 1;
            aux2 = aux2 << 1;

            if (delay(1) == 0) {
                turnOff();
                return;
            }
        }
    }
}

struct termios modifyTerminalConfig(void) {
    struct termios oldattr, newattr;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    return oldattr;
}

void restoreTerminalConfig(struct termios oldattr) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
}

bool keyHit(int index) {
    struct termios oldattr = modifyTerminalConfig();
    int ch, oldf;
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();

    if(ch == 119) {
        if(delayTime[index] > 1000) {
            delayTime[index] = delayTime[index] - 1000;
        }
    }

    if(ch == 115) {
        delayTime[index] = delayTime[index] + 1000;
    }

    restoreTerminalConfig(oldattr);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == 27) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void pinSetup(void) {
    for (int i = 0; i < 8; i++) {
        // Configura los pines LED según tu plataforma
    }
}

void ledShow(unsigned char output) {
    for (int j = 0; j < 8; j++) {
        // Maneja los LEDs según tu plataforma
    }
}

int delay(int index) {
    for(int i = delayTime[index]; i > 0; --i) {
        if (keyHit(index)) {
            return 0;
        }
    }
    return 1;
}

void clearInputBuffer() {
    printf("Presione ENTER para confirmar\n");
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Descarta caracteres del buffer de entrada
    }
}

void turnOff() {
    unsigned char off = 0x0;
    ledShow(off);
}
