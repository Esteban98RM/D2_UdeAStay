#include "huesped.h"

// Constructor por defecto
Huesped::Huesped() : documento(""), clave(""), antiguedad(0), puntuacion(0.0f) {}

// Constructor con parametros
Huesped::Huesped(const string& doc, const string& clv, int ant, float punt) :
    documento(doc), clave(clv), antiguedad(ant), puntuacion(punt) {

    // Validación básica de datos
    if (documento.empty() || clave.empty()) {
        throw invalid_argument("Datos del huesped no pueden estar vacios");
    }
    if (antiguedad < 0) {
        throw invalid_argument("Antiguedad no puede ser negativa");
    }
    if (puntuacion < 0.0f || puntuacion > 5.0f) {
        throw invalid_argument("Puntuacion debe estar entre 0.0 y 5.0");
    }
}

// Nota: No necesitamos destructor explícito ya que std::string maneja su propia memoria
