.global retardo
retardo:
    subs r0, r0, #1   // Resta 1 de r0 y actualiza las banderas
    bne retardo       // Si r0 no es cero, salta a la etiqueta 'retardo'
    bx lr             // Retorna de la función
