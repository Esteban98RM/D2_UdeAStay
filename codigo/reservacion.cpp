#include "reservacion.h"

#include <cstring>

const size_t MAX_CARACTERES = 1000;

// Constructor por defecto
Reservacion::Reservacion() :
    codigo(""), fecha_entrada(""), duracion_estadia(0), codigo_alojamiento(""),
    documento(""), metodo_pago(' '), fecha_pago(""), monto(0),
    anotacion(nullptr), capacidad_anotacion(0) {

    reservarEspacio(1); // Espacio para string vacío
    anotacion[0] = '\0';
}

// Constructor con parámetros
Reservacion::Reservacion(const string& cod, const string& f_entrada, int duracion, const string& cod_aloja,
                         const string& doc_huesped, char metodo, const string& f_pago, float monto_reserva, const string& notas) :
    codigo(cod), fecha_entrada(f_entrada), duracion_estadia(duracion), codigo_alojamiento(cod_aloja),
    documento(doc_huesped), metodo_pago(metodo), fecha_pago(f_pago), monto(monto_reserva), anotacion(nullptr), capacidad_anotacion(0) {
    setAnotacion(notas); // Usamos el setter para manejar la asignacion
}

// Constructor de copia
Reservacion::Reservacion(const Reservacion& otro) :
    codigo(otro.codigo), fecha_entrada(otro.fecha_entrada), duracion_estadia(otro.duracion_estadia), codigo_alojamiento(otro.codigo_alojamiento),
    documento(otro.documento), metodo_pago(otro.metodo_pago), fecha_pago(otro.fecha_pago), monto(otro.monto), anotacion(nullptr), capacidad_anotacion(0) {
    copiarAnotacion(otro.anotacion);
}

// Sobrecarga del operador de asignación
Reservacion& Reservacion::operator=(const Reservacion& otro) {
    if (this != &otro) {
        // Copiar miembros básicos
        codigo = otro.codigo;
        fecha_entrada = otro.fecha_entrada;
        duracion_estadia = otro.duracion_estadia;
        codigo_alojamiento = otro.codigo_alojamiento;
        documento = otro.documento;
        metodo_pago = otro.metodo_pago;
        fecha_pago = otro.fecha_pago;
        monto = otro.monto;

        // Copiar anotacion
        copiarAnotacion(otro.anotacion);
    }
    return *this;
}

// Destructor
Reservacion::~Reservacion() {
    delete[] anotacion;
}

// Metodos privados para manejo de memoria
void Reservacion::reservarEspacio(size_t nueva_capacidad) {
    delete[] anotacion;
    capacidad_anotacion = nueva_capacidad;
    anotacion = new char[capacidad_anotacion];
}

void Reservacion::copiarAnotacion(const char* nueva_anotacion) {
    if (!nueva_anotacion) {
        reservarEspacio(1);
        anotacion[0] = '\0';
        return;
    }

    size_t longitud = strlen(nueva_anotacion);
    if (longitud > MAX_CARACTERES) {
        longitud = MAX_CARACTERES;
    }

    reservarEspacio(longitud + 1);
    strncpy(anotacion, nueva_anotacion, longitud);
    anotacion[longitud] = '\0';
}

// Métodos públicos
string Reservacion::getAnotacion() const {
    return anotacion ? string(anotacion) : "";
}

void Reservacion::setAnotacion(const string& nueva_anotacion) {
    size_t longitud = nueva_anotacion.length();
    if (longitud > MAX_CARACTERES) {
        longitud = MAX_CARACTERES;
    }

    reservarEspacio(longitud + 1);
    strncpy(anotacion, nueva_anotacion.c_str(), longitud);
    anotacion[longitud] = '\0';
}
