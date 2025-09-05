#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#define TAMANIO 5

typedef struct
{
    char id;
    int tiempo;
    int tiempoServicio;
} proceso;

typedef struct
{
    char datosProcesamiento;
    int instanteLlegada;
    int instanteFinal;
} instancia;

/// @brief Carga los tiempos de los procesos.
/// @param procesos Arreglo de procesos.
void cargarTiempoProcesos(proceso procesos[])
{
    int i = 0, validador, tiempoProceso;
    printf("\n");
    while (i < TAMANIO)
    {
        printf(ANSI_bBLUE "Ingresar las unidades de tiempo del proceso %c: " ANSI_YELLOW, procesos[i].id);
        validador = scanf("%d", &tiempoProceso);
        vaciar_buffer();
        while (validador != 1 || tiempoProceso < 1 || tiempoProceso > 100)
        {
            printf(ANSI_RED "\t\t-------- ERROR -------- \n");
            printf("\t\tDATO FUERA DE RANGO\n\n" ANSI_RESET);
            pausa();
            printf(ANSI_bBLUE "Ingresar las unidades de tiempo del proceso %c: " ANSI_YELLOW, procesos[i].id);
            validador = scanf("%d", &tiempoProceso);
            vaciar_buffer();
        }
        procesos[i].tiempo = tiempoProceso;
        procesos[i].tiempoServicio = tiempoProceso;
        i++;
    }
}

/// @brief Carga los nombres de los procesos.
/// @param procesos Arreglo de procesos.
void cargarNombreProcesos(proceso procesos[])
{
    for (int i = 0; i < TAMANIO; i++)
    {
        procesos[i].id = 'A' + i;
    }
}

/// @brief Calcula la cantidad de servicio de los procesos.
/// @param procesos Arreglo de procesos.
/// @return Cantidad de servicio.
int calcularCantidadServicio(proceso procesos[], int quantum, int conmutacion)
{
    int acumulador = 0;
    for (int i = 0; i < TAMANIO; ++i)
    {
        if (procesos[i].tiempo % quantum == 0)
            acumulador += procesos[i].tiempo / quantum;
        else
            acumulador += procesos[i].tiempo / quantum + 1;
    }
    return acumulador;
}

/// @brief Implementa el algoritmo Round Robin.
/// @param procesos Arreglo de procesos.
/// @param instancias Arreglo de instancias.
void roundRobin(proceso procesos[], instancia instancias[], int quantum, int conmutacion)
{
    int tiempo = 0;
    int j = 0;
    bool indicadorFinal = false, indicadorVacio;
    while (indicadorFinal == false)
    {
        indicadorVacio = false;
        for (int i = 0; i < TAMANIO; ++i)
        {
            if (procesos[i].tiempo > 0)
            {
                indicadorVacio = true;
                instancias[j].datosProcesamiento = procesos[i].id;
                instancias[j].instanteLlegada = tiempo;
                if (quantum > procesos[i].tiempo)
                {
                    tiempo += procesos[i].tiempo;
                    procesos[i].tiempo = 0;
                }
                else
                {
                    tiempo += quantum;
                    procesos[i].tiempo -= quantum;
                }
                instancias[j].instanteFinal = tiempo;
                tiempo += conmutacion;
                j++;
            }
        }
        if (indicadorVacio == false)
            indicadorFinal = true;
    }
}

/// @brief Calcula el tiempo de retorno y el tiempo de espera.
/// @param dato Identificador del proceso.
/// @param total Total de instancias.
/// @param tiempoLlegada Tiempo de llegada del proceso.
/// @param instancias Arreglo de instancias.
/// @param tiempoServicio Tiempo de servicio del proceso.
void tiempos(char dato, int total, int tiempoLlegada, instancia instancias[], int tiempoServicio)
{
    // Busco el instante en el que termina el proceso.
    bool encontre = false;
    int i = total - 1;
    while (encontre == false && i >= 0)
    {
        if (instancias[i].datosProcesamiento == dato)
            encontre = true;
        else
            i--;
    }
    int tiempoFinal = instancias[i].instanteFinal;
    /*
    // Busco el instante en el que comienza el proceso.
    encontre = false;
    i = 0;
    while (encontre == false)
    {
        if (instancias[i].datosProcesamiento == dato)
            encontre = true;
        else
            i++;
    }
    int tiempoInicio = instancias[i].instanteLlegada;
    */
    int retorno, espera;
    retorno = tiempoFinal - tiempoLlegada; // Tiempo de retorno.
    espera = retorno - tiempoServicio;     // Tiempo de espera.
    printf(ANSI_bBLUE "Proceso: " ANSI_bGREEN "%c" ANSI_bCYAN " -->" ANSI_bBLUE " Retorno: " ANSI_bGREEN "%d" ANSI_bBLUE ", Espera: " ANSI_bGREEN "%d\n" ANSI_RESET, dato, retorno, espera);
}

/// @brief Muestra el procesamiento de los procesos.
/// @param total Total de instancias.
/// @param instancias Arreglo de instancias.
void mostrarProcesamiento(int total, instancia instancias[])
{
    for (int i = 0; i < total; i++)
        printf(ANSI_bBLUE "Proceso: " ANSI_bGREEN "%c" ANSI_bCYAN " -->" ANSI_bBLUE " Tiempo entrada: " ANSI_bGREEN "%d" ANSI_bBLUE ", Tiempo salida: " ANSI_bGREEN "%d\n" ANSI_RESET, instancias[i].datosProcesamiento, instancias[i].instanteLlegada, instancias[i].instanteFinal);
}

/// @brief Muestra los procesos.
/// @param procesos Arreglo de procesos.
/// @param quantum Valor del quantum.
/// @param conmutacion Valor de la conmutacion.
void mostrarProcesos(proceso procesos[], int quantum, int conmutacion)
{
    for (int i = 0; i < TAMANIO; i++)
        printf(ANSI_bBLUE "Proceso: " ANSI_bGREEN "%c" ANSI_bCYAN " -->" ANSI_bBLUE " Tiempo inicial: " ANSI_bGREEN "%d" ANSI_bBLUE ", Tiempo de servicio: " ANSI_bGREEN "%d\n" ANSI_RESET, procesos[i].id, 0, procesos[i].tiempo);
    printf(ANSI_bMAGENTA "\nQuantum: " ANSI_bGREEN "%d\n" ANSI_RESET, quantum);
    printf(ANSI_bMAGENTA "Conmutacion: " ANSI_bGREEN "%d" ANSI_RESET, conmutacion);
}

/// @brief Muestra los tiempos de los procesos.
/// @param total Total de instancias.
/// @param procesos Arreglo de procesos.
/// @param instancias Arreglo de instancias.
void mostrarTiempos(int total, proceso procesos[], instancia instancias[])
{
    for (int i = 0; i < TAMANIO; ++i)
    {
        tiempos(procesos[i].id, total, 0, instancias, procesos[i].tiempoServicio);
    }
}

/// @brief Cambia el valor del quantum.
/// @param quantum Valor actual del quantum.
/// @return Nuevo valor del quantum.
int cambiarQuantum(int quantum)
{
    int nuevoQuantum;
    printf(ANSI_bBLUE "Ingresar el nuevo valor de quantum (actual %d): " ANSI_YELLOW, quantum);
    int validador = scanf("%d", &nuevoQuantum);
    vaciar_buffer();
    while (validador != 1 || nuevoQuantum < 1 || nuevoQuantum > 100)
    {
        printf(ANSI_RED "\t\t-------- ERROR -------- \n");
        printf("\t\tDATO FUERA DE RANGO\n\n" ANSI_RESET);
        pausa();
        printf(ANSI_bBLUE "Ingresar el nuevo valor de quantum (actual %d): " ANSI_YELLOW, quantum);
        validador = scanf("%d", &nuevoQuantum);
        vaciar_buffer();
    }
    printf(ANSI_GREEN "\t\t-------- EXITO -------- \n");
    printf("\t\tQUANTUM MODIFICADO\n\n" ANSI_RESET);
    return nuevoQuantum;
}

/// @brief Cambia el valor de la conmutacion.
/// @param conmutacion Valor actual de la conmutacion.
/// @return Nuevo valor de la conmutacion.
int cambiarConmutacion(int conmutacion)
{
    int nuevaConmutacion;
    printf(ANSI_bBLUE "Ingresar el nuevo valor de conmutacion (actual %d): " ANSI_YELLOW, conmutacion);
    int validador = scanf("%d", &nuevaConmutacion);
    vaciar_buffer();
    while (validador != 1 || nuevaConmutacion < 1 || nuevaConmutacion > 100)
    {
        printf(ANSI_RED "\t\t-------- ERROR -------- \n");
        printf("\t\tDATO FUERA DE RANGO\n\n" ANSI_RESET);
        pausa();
        printf(ANSI_bBLUE "Ingresar el nuevo valor de conmutacion (actual %d): " ANSI_YELLOW, conmutacion);
        validador = scanf("%d", &nuevaConmutacion);
        vaciar_buffer();
    }
    printf(ANSI_GREEN "\t\t-------- EXITO -------- \n");
    printf("\t\tCONMUTACION MODIFICADA\n\n" ANSI_RESET);
    return nuevaConmutacion;
}

void menu_principal()
{
    printf("\n");
    printf(ANSI_bBLUE "  ============================================================================\n");
    printf(" |                                 ROUND ROBIN                                 |\n");
    printf("  ============================================================================\n");
    printf("\n");
    printf("  1   Ingresar tiempo de proceso\n");
    printf("  2   Ingresar quantum\n");
    printf("  3   Ingresar tiempo de conmutacion\n");
    printf("  4   Ejecutar Round robin y mostrar estadisticas\n");
    printf("\n");
    printf("  0   Salir\n");
    printf("\n");
    printf(" ------------------------------------------------------------------------------\n");
    printf("\n");
    printf("  Por favor seleccione una opcion: " ANSI_YELLOW);
}

int main()
{
    int opcion;
    int quantum = 4;
    int conmutacion = quantum / 4;
    bool tiemposCargados = false;
    bool salir = false;
    proceso procesos[TAMANIO];
    cargarNombreProcesos(procesos);
    limpiar_pantalla();
    while (!salir)
    {
        menu_principal();
        int validador = scanf("%i", &opcion);
        while (validador != 1 || opcion < 0 || opcion > 4)
        {
            printf(ANSI_RED "Opcion incorrecta\n" ANSI_RESET);
            printf(ANSI_bBLUE "  Por favor seleccione una opcion: " ANSI_YELLOW);
            while (getchar() != '\n')
                ;
            validador = scanf("%i", &opcion);
        }
        switch (opcion)
        {
        case 1:
            cargarTiempoProcesos(procesos);
            tiemposCargados = true;
            pausa();
            break;
        case 2:
            quantum = cambiarQuantum(quantum);
            pausa();
            break;
            menu_principal();
        case 3:
            conmutacion = cambiarConmutacion(conmutacion);
            pausa();
            break;
        case 4:
        {
            if (tiemposCargados == true)
            {
                printf(ANSI_bMAGENTA "\n\nTabla de procesos:\n\n" ANSI_RESET);
                mostrarProcesos(procesos, quantum, conmutacion);
                // Tabla de procesamiento
                int total = calcularCantidadServicio(procesos, quantum, conmutacion);
                instancia instancias[total];
                // Round Robin
                roundRobin(procesos, instancias, quantum, conmutacion);
                // Mostrar estadisticas
                printf(ANSI_bMAGENTA "\n\n\nProcesamiento:\n\n" ANSI_RESET);
                mostrarProcesamiento(total, instancias);
                printf(ANSI_bMAGENTA "\n\nTiempos:\n\n" ANSI_RESET);
                mostrarTiempos(total, procesos, instancias);
                tiemposCargados = false; // Forzar a que se vuelvan a cargar los tiempos
            }
            else
            {
                printf(ANSI_RED "\t\t-------- ERROR -------- \n");
                printf("\t\tTIEMPOS NO CARGADOS\n\n" ANSI_RESET);
            }
            vaciar_buffer();
            pausa();
            break;
        }
        case 0:
            printf(ANSI_RESET);
            salir = true;
            break;
        }
    }
    return 0;
}