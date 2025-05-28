#include "reservacion.h"
#include "alojamiento.h"
#include "sistema.h"
#include "fecha.h"
#include "huesped.h"

#include <cstring>
#include <iomanip>

const size_t MAX_CARACTERES = 1000;

// Constructor por defecto
Reservacion::Reservacion() :
    codigo(""), fecha_entrada(""), duracion_estadia(0), codigo_alojamiento(""),
    documento(""), metodo_pago(' '), fecha_pago(""), monto(0),
    anotacion(nullptr), capacidad_anotacion(0) {

    reservarEspacio(1); // Espacio para string vacio
    anotacion[0] = '\0';
}

// Constructor con parametros
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

// Sobrecarga del operador de asignacion
Reservacion& Reservacion::operator=(const Reservacion& otro) {
    if (this != &otro) {
        // Copiar miembros basicos
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
    anotacion = nullptr;
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

// Metodos publicos
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

// Alojamiento* Reservacion::filtrarDisponiblesPorFecha(
//     const Alojamiento* candidatos, int numCandidatos,
//     const Fecha& entrada, int noches,
//     const Reservacion* todasReservas, int numReservas,
//     int& cantidadFiltrada) {

//     Alojamiento* disponibles = new Alojamiento[numCandidatos];
//     cantidadFiltrada = 0;

//     for (int i = 0; i < numCandidatos; ++i) {
//         if (candidatos[i].estaDisponible(candidatos[i], entrada, noches,
//                                          todasReservas, numReservas)) {
//             disponibles[cantidadFiltrada++] = candidatos[i];
//         }
//     }

//     return disponibles;
// }

// Implementacion del filtro de disponibilidad
// Alojamiento* Reservacion::filtrarDisponiblesPorFecha(Alojamiento* candidatos,
//                                                      int nCandidatos,
//                                                      const Fecha& fechaEntrada,
//                                                      int noches,
//                                                      const Reservacion* reservaciones,
//                                                      int nReservaciones,
//                                                      int& nDisponibles) {
//     Alojamiento* disponibles = new Alojamiento[nCandidatos];
//     nDisponibles = 0;

//     for (int i = 0; i < nCandidatos; ++i) {
//         bool estaDisponible = true;

//         // Verificar conflictos con reservaciones existentes
//         for (int j = 0; j < nReservaciones && estaDisponible; ++j) {
//             if (reservaciones[j].getCodigoAlojamiento() == candidatos[i].getCodigo()) {
//                 Fecha fechaReservada(reservaciones[j].getFechaEntrada());
//                 if (hayConflictoFechas(fechaEntrada, noches,
//                                        fechaReservada, reservaciones[j].getDuracion())) {
//                     estaDisponible = false;
//                 }
//             }
//         }

//         if (estaDisponible) {
//             disponibles[nDisponibles++] = candidatos[i];
//         }
//     }

//     return disponibles;
// }

Alojamiento* Reservacion::filtrarDisponiblesPorFecha(Alojamiento* candidatos,
                                                     int nCandidatos,
                                                     const Fecha& fechaEntrada,
                                                     int noches,
                                                     const Reservacion* reservaciones,
                                                     int nReservaciones,
                                                     int& nDisponibles) {
    Alojamiento* disponibles = new Alojamiento[nCandidatos];
    nDisponibles = 0;

    cout << "\n=== DEBUG: Filtrado por fecha ===" << endl;
    cout << "Fecha buscada: " << fechaEntrada.toString() << " por " << noches << " noches" << endl;
    cout << "Total reservaciones existentes: " << nReservaciones << endl;

    for (int i = 0; i < nCandidatos; ++i) {
        cout << "\nEvaluando alojamiento: " << candidatos[i].getCodigo() << endl;
        bool estaDisponible = true;

        for (int j = 0; j < nReservaciones && estaDisponible; ++j) {
            if (reservaciones[j].getCodigoAlojamiento() == candidatos[i].getCodigo()) {
                Fecha fechaReservada(reservaciones[j].getFechaEntrada());
                cout << "Comparando con reserva existente: " << reservaciones[j].getCodigo() << endl;
                cout << "Fecha reservada: " << fechaReservada.toString() << " por "
                     << reservaciones[j].getDuracion() << " noches" << endl;

                if (hayConflictoFechas(fechaEntrada, noches,
                                       fechaReservada, reservaciones[j].getDuracion())) {
                    estaDisponible = false;
                    cout << "CONFLICTO ENCONTRADO" << endl;
                }
            }
        }

        if (estaDisponible) {
            disponibles[nDisponibles++] = candidatos[i];
            cout << "ALOJAMIENTO DISPONIBLE" << endl;
        } else {
            cout << "ALOJAMIENTO NO DISPONIBLE" << endl;
        }
    }

    cout << "Total disponibles: " << nDisponibles << endl;
    return disponibles;
}

// bool Reservacion::hayConflictoFechas(const Fecha& fechaEntrada1, int noches1,
//                                      const Fecha& fechaEntrada2, int noches2) {
//     // Convertir fechas a dias desde una fecha base para facilitar comparacion
//     int inicio1 = fechaEntrada1.aDiasDesdeFechaBase();
//     int fin1 = inicio1 + noches1 - 1;
//     int inicio2 = fechaEntrada2.aDiasDesdeFechaBase();
//     int fin2 = inicio2 + noches2 - 1;

//     // Verificar si hay solapamiento
//     return !(fin1 < inicio2 || fin2 < inicio1);
// }

bool Reservacion::hayConflictoFechas(const Fecha& fechaEntrada1, int noches1,
                                     const Fecha& fechaEntrada2, int noches2) {
    // Debug: Mostrar las fechas que se estan comparando
    cout << "\n=== DEBUG: Comparando fechas ===" << endl;
    cout << "Fecha 1: " << fechaEntrada1.toString() << " por " << noches1 << " noches" << endl;
    cout << "Fecha 2: " << fechaEntrada2.toString() << " por " << noches2 << " noches" << endl;

    int inicio1 = fechaEntrada1.aDiasDesdeFechaBase();
    int fin1 = inicio1 + noches1 - 1;
    int inicio2 = fechaEntrada2.aDiasDesdeFechaBase();
    int fin2 = inicio2 + noches2 - 1;

    // Debug: Mostrar los calculos
    cout << "Inicio1: " << inicio1 << ", Fin1: " << fin1 << endl;
    cout << "Inicio2: " << inicio2 << ", Fin2: " << fin2 << endl;

    bool conflicto = !(fin1 < inicio2 || fin2 < inicio1);
    cout << "Hay conflicto? " << (conflicto ? "Si" : "No") << endl;

    return conflicto;
}

void Reservacion::mostrarDetalle(int numero, bool esParaAnfitrion, Sistema* sistema) const {
    Fecha fechaEntrada(this->getFechaEntrada());
    Fecha fechaFinal = fechaEntrada.sumarDias(this->getDuracion());

    cout << "--- RESERVACION " << numero << " ---\n";
    cout << "Codigo: " << this->getCodigo() << "\n";

    if (esParaAnfitrion) {
        // Vista para anfitrion: incluye datos del huesped
        cout << "Alojamiento: " << this->getCodigoAlojamiento() << "\n";
        cout << "Huesped: " << obtenerNombreHuesped(sistema) << "\n";
        cout << "Documento Huesped: " << this->getDocumento() << "\n";
    } else {
        // Vista para huesped: incluye estado y metodo de pago
        cout << "Alojamiento: " << this->getCodigoAlojamiento() << "\n";
        cout << "Estado: " << (fechaFinal.yaPaso() ? "COMPLETADA" : "PENDIENTE") << "\n";
        cout << "Metodo de Pago: ";

        switch(this->getMetodoPago()) {
        case 'T':
        case 't':
            cout << "Tarjeta de Credito\n";
            break;
        case 'P':
        case 'p':
            cout << "PSE\n";
            break;
        default:
            cout << "No especificado\n";
            break;
        }
        cout << "Fecha de Pago: " << this->getFechaPago() << "\n";
    }

    cout << "Fecha Entrada: " << fechaEntrada.toString() << "\n";
    cout << "Fecha Salida: " << fechaFinal.toString() << "\n";
    cout << "Duracion: " << this->getDuracion() << " noches\n";
    cout << "Monto: $" << fixed << setprecision(0) << this->getMonto() << "\n";

    string anotacion = this->getAnotacion();
    if (!anotacion.empty()) {
        cout << "Anotaciones: " << anotacion << "\n";
    }
    cout << "--------------------------------\n";
}

string Reservacion::obtenerNombreHuesped(Sistema* sistema) const {
    const Huesped* huespedes = sistema->getHuespedes();
    int totalHuespedes = sistema->getNumHuespedes();

    for (int i = 0; i < totalHuespedes; i++) {
        if (huespedes[i].getDocumento() == this->getDocumento()) {
            return huespedes[i].getNombre();
        }
    }
    return "No encontrado";
}
