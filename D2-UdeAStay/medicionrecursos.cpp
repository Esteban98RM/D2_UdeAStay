#include "medicionrecursos.h"
#include <iostream>

long long contadorIteraciones = 0;
long long memoriaEstimacion = 0;

void inicializarContador() {
    contadorIteraciones = 0;
    memoriaEstimacion = 0;
}

void incrementarIteracion() {
    contadorIteraciones++;
}

long long obtenerIteraciones() {
    return contadorIteraciones;
}

void agregarMemoria(long long bytes) {
    memoriaEstimacion += bytes;
}

long long obtenerMemoriaEstimacion() {
    return memoriaEstimacion;
}

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
