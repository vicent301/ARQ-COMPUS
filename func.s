
.global secuencia_formula1
.global delay
.global ledShow
.global turnOff

.equ LED_COUNT, 8

.section .text

secuencia_formula1:
    PUSH    {LR}             @ Guardar el valor de retorno

main_loop:
    LDR     R0, =leds        @ Cargar la dirección del array de LEDs
    MOV     R1, #LED_COUNT   @ Número de LEDs

    @ Encender luces de izquierda a derecha
left_to_right:
    LDRB    R2, [R0], #1     @ Cargar el siguiente valor de LED y avanzar el puntero
    BL      ledShow          @ Mostrar el LED
    BL      delay            @ Llamar a la función de retardo
    CMP     R0, #leds + LED_COUNT @ Comparar si hemos llegado al final del array
    BNE     left_to_right    @ Si no, repetir

    @ Encender luces de derecha a izquierda
    SUB     R0, R0, #2       @ Retroceder dos posiciones en el array
right_to_left:
    LDRB    R2, [R0], #-1    @ Cargar el valor de LED y retroceder el puntero
    BL      ledShow          @ Mostrar el LED
    BL      delay            @ Llamar a la función de retardo
    CMP     R0, #leds        @ Comparar si hemos llegado al principio del array
    BGT     right_to_left    @ Si no, repetir

    B       main_loop        @ Repetir la secuencia principal

    POP     {PC}             @ Restaurar el valor de retorno y regresar

leds:
    .byte 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80



.global sirena
sirena:

PUSH {R4, R5, R6, LR}

reset:
LDR R4, =sirenad
MOV r6, #21

loop:
LDRB R5, [R4], #1

MOV R0, R5
BL disp_binary //display on screen

MOV R0, R5
BL ledShow

MOV R0, #3 //Index for array in c
BL delay

SUBS R6, R6, #1
BEQ reset

CMP R0, #0
BNE loop
BL turnOff

POP {R4, R5, R6, PC}


.data
sirenad:
.byte 0x0
.byte 0xF
.byte 0xF
.byte 0xF0
.byte 0xF0
.byte 0x0
.byte 0x1
.byte 0x2
.byte 0x4
.byte 0x8
.byte 0x10
.byte 0x20
.byte 0x40
.byte 0x80
.byte 0x40
.byte 0x20
.byte 0x10
.byte 0x8
.byte 0x4
.byte 0x2
.byte 0x1
