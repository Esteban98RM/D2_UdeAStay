#ifndef MEDICIONRECURSOS_H
#define MEDICIONRECURSOS_H

extern long long contadorIteraciones;
extern long long memoriaEstimacion;

void inicializarContador();
void incrementarIteracion();
long long obtenerIteraciones();

void agregarMemoria(long long bytes);
long long obtenerMemoriaEstimacion();

void mostrarEstadisticasRecursos();

#endif // MEDICIONRECURSOS_H
