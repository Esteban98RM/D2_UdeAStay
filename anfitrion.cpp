#include "anfitrion.h"

using namespace std;

// Constructor por defecto
Anfitrion::Anfitrion() : codigo(""), documento(""), clave(""), antiguedad(0), puntuacion(0.0f) {}

// Constructor con parametros
Anfitrion::Anfitrion(const string& cod, const string& doc, const string& clv, int ant, float punt) :
    codigo(cod), documento(doc), clave(clv), antiguedad(ant), puntuacion(punt) {

    // Validacion basica de datos
    if (codigo.empty() || documento.empty() || clave.empty()) {
        throw invalid_argument("Datos del anfitrion no pueden estar vacios");
    }
    if (antiguedad < 0) {
        throw invalid_argument("Antiguedad no puede ser negativa");
    }
    if (puntuacion < 0.0f || puntuacion > 5.0f) {
        throw invalid_argument("Puntuacion debe estar entre 0.0 y 5.0");
    }
}

// Nota: No necesitamos destructor explícito ya que string maneja su propia memoria
