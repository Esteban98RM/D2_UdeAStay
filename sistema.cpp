#include "sistema.h"
#include "anfitrion.h"
#include "huesped.h"
#include "gestionarchivos.h"

#include <fstream>
#include <sstream>

// Constructores
Sistema::Sistema() : documento(""), anfitriones(nullptr), huespedes(nullptr),
    alojamientos(nullptr), reservaciones(nullptr), numAnfitriones(0),
    numHuespedes(0), numAlojamientos(0), numReservaciones(0) {}

Sistema::Sistema(const string& doc) : Sistema() {
    documento = doc;
}

Sistema::Sistema(const string& doc, Anfitrion* anf, Huesped* hue) : Sistema(doc) {
    // Implementación para manejar los punteros a anfitriones y huéspedes
}

// Destructor
Sistema::~Sistema() {
    liberarMemoria();
}

// Operador de comparacion
bool Sistema::operator==(const Sistema& otro) const {
    return documento == otro.documento;
}

// Carga de datos usando GestionArchivos
void Sistema::cargarDatos() {
    GestionArchivos gestor;

    // Cargar anfitriones
    gestor.cargarAnfitriones(anfitriones, numAnfitriones);

    // Cargar huespedes
    gestor.cargarHuespedes(huespedes, numHuespedes);

    // Cargar alojamientos
    gestor.cargarAlojamientos(alojamientos, numAlojamientos);

    // Cargar reservaciones
    gestor.cargarReservaciones(reservaciones, numReservaciones);
}

// Autenticacion con clave
bool Sistema::autenticarUsuario(const string& doc, const string& clave, string& tipoRol) {
    // Buscar en anfitriones
    for (int i = 0; i < numAnfitriones; i++) {
        if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == clave) {
            tipoRol = "Anfitrion";
            return true;
        }
    }

    // Buscar en huespedes
    for (int i = 0; i < numHuespedes; i++) {
        if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == clave) {
            tipoRol = "Huesped";
            return true;
        }
    }

    return false; // No encontrado
}

// Liberación de memoria
void Sistema::liberarMemoria() {
    delete[] anfitriones;
    delete[] huespedes;
    delete[] alojamientos;
    delete[] reservaciones;

    anfitriones = nullptr;
    huespedes = nullptr;
    alojamientos = nullptr;
    reservaciones = nullptr;

    numAnfitriones = numHuespedes = numAlojamientos = numReservaciones = 0;
}
