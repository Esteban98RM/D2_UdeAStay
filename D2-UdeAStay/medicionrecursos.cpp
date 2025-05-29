#include "medicionrecursos.h"
#include <iostream>

long long contadorIteraciones = 0;
long long memoriaEstimacion = 0;

/**
 * @file medicionrecursos.cpp
 * @brief Funciones para el conteo de iteraciones y estimación de uso de memoria.
 */

/**
 * @brief Inicializa los contadores de iteraciones y memoria estimada a cero.
 */
void inicializarContador() {
    contadorIteraciones = 0;
    memoriaEstimacion = 0;
}

/**
 * @brief Incrementa el contador de iteraciones en 1.
 */
void incrementarIteracion() {
    contadorIteraciones++;
}

/**
 * @brief Obtiene el valor actual del contador de iteraciones.
 * @return Número total de iteraciones realizadas.
 */
long long obtenerIteraciones() {
    return contadorIteraciones;
}

/**
 * @brief Suma una cantidad dada de bytes a la memoria estimada utilizada.
 * @param bytes Cantidad de bytes a agregar.
 */
void agregarMemoria(long long bytes) {
    memoriaEstimacion += bytes;
}

/**
 * @brief Obtiene la cantidad total estimada de memoria utilizada en bytes.
 * @return Memoria estimada en bytes.
 */
long long obtenerMemoriaEstimacion() {
    return memoriaEstimacion;
}

/**
 * @brief Muestra por consola las estadísticas actuales de iteraciones y memoria estimada.
 *        Convierte la memoria a megabytes si supera 1 MB.
 */
void mostrarEstadisticasRecursos() {
    std::cout << "\n==========================================\n";
    std::cout << "         ESTADISTICAS DE RECURSOS\n";
    std::cout << "==========================================\n";
    std::cout << "Iteraciones totales: " << contadorIteraciones << "\n";

    std::cout << "Memoria estimada utilizada: " << memoriaEstimacion << " bytes";
    if (memoriaEstimacion > 1024 * 1024) {
        double mb = memoriaEstimacion / (1024.0 * 1024.0);
        std::cout << " (" << mb << " MB)";
    }
    std::cout << "\n==========================================\n";
}
