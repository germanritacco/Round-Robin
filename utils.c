#include <stdio.h>
#include "utils.h"

/// @brief Vacía el buffer de entrada.
/// @param void No recibe parámetros.
void vaciar_buffer(void)
{
    char nada[200];
    fgets(nada, 200, stdin);
}

/// @brief Frena la ejecución del código esperando el ingreso de un carácter.
/// @param void No recibe parámetros.
void pausa(void)
{
    printf(ANSI_bCYAN "\n\nPresione una tecla para continuar...." ANSI_RESET);
    getchar();
}

/// @brief Limpia la pantalla de la consola.
void limpiar_pantalla()
{
    printf("\033[2J\033[1;1H");
}
