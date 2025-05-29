#include "reservacion.h"
#include "alojamiento.h"
#include "medicionrecursos.h"
#include "sistema.h"
#include "fecha.h"
#include "huesped.h"

#include <cstring>
#include <iomanip>

const size_t MAX_CARACTERES = 1000;

/**
 * @brief Constructor por defecto de la clase Reservacion.
 *
 * Inicializa todos los atributos con valores predeterminados.
 * Reserva espacio en memoria para la anotación y establece su valor como cadena vacía.
 */
Reservacion::Reservacion() :
    codigo(""), fecha_entrada(""), duracion_estadia(0), codigo_alojamiento(""),
    documento(""), metodo_pago(' '), fecha_pago(""), monto(0),
    anotacion(nullptr), capacidad_anotacion(0) {

    incrementarIteracion();
    reservarEspacio(1); // Espacio para string vacio
    anotacion[0] = '\0';
    agregarMemoria(1);
}

/**
 * @brief Constructor parametrizado de la clase Reservacion.
 *
 * @param cod Código de la reservación.
 * @param f_entrada Fecha de entrada en formato string.
 * @param duracion Duración de la estadía en noches.
 * @param cod_aloja Código del alojamiento reservado.
 * @param doc_huesped Documento del huésped.
 * @param metodo Método de pago ('T' para tarjeta, 'P' para PSE, etc.).
 * @param f_pago Fecha del pago.
 * @param monto_reserva Monto pagado por la reserva.
 * @param notas Anotaciones adicionales sobre la reservación.
 */
Reservacion::Reservacion(const string& cod, const string& f_entrada, int duracion, const string& cod_aloja,
                         const string& doc_huesped, char metodo, const string& f_pago, float monto_reserva, const string& notas) :
    codigo(cod), fecha_entrada(f_entrada), duracion_estadia(duracion), codigo_alojamiento(cod_aloja),
    documento(doc_huesped), metodo_pago(metodo), fecha_pago(f_pago), monto(monto_reserva), anotacion(nullptr), capacidad_anotacion(0) {

    incrementarIteracion();
    setAnotacion(notas); // Usamos el setter para manejar la asignacion
    agregarMemoria(cod.capacity() + f_entrada.capacity() + cod_aloja.capacity() +
    doc_huesped.capacity() + f_pago.capacity());
}

/**
 * @brief Constructor de copia de la clase Reservacion.
 *
 * @param otro Otra instancia de Reservacion desde la cual se copiarán los datos.
 */
Reservacion::Reservacion(const Reservacion& otro) :
    codigo(otro.codigo), fecha_entrada(otro.fecha_entrada), duracion_estadia(otro.duracion_estadia), codigo_alojamiento(otro.codigo_alojamiento),
    documento(otro.documento), metodo_pago(otro.metodo_pago), fecha_pago(otro.fecha_pago), monto(otro.monto), anotacion(nullptr), capacidad_anotacion(0) {

    incrementarIteracion();
    copiarAnotacion(otro.anotacion);
    agregarMemoria(codigo.capacity() + fecha_entrada.capacity() + codigo_alojamiento.capacity() +
    documento.capacity() + fecha_pago.capacity());
    if (anotacion) {
        agregarMemoria(capacidad_anotacion);
    }
}

/**
 * @brief Sobrecarga del operador de asignación.
 *
 * @param otro Otra instancia de Reservacion a asignar.
 * @return Referencia a la instancia actual con los datos asignados.
 */
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

/**
 * @brief Destructor de la clase Reservacion.
 *
 * Libera la memoria dinámica asignada para la anotación.
 */
Reservacion::~Reservacion() {
    delete[] anotacion;
    anotacion = nullptr;
}

/**
 * @brief Reserva espacio dinámico para almacenar anotaciones.
 *
 * @param nueva_capacidad Nueva capacidad en caracteres para el arreglo de anotación.
 */
void Reservacion::reservarEspacio(size_t nueva_capacidad) {
    delete[] anotacion;
    capacidad_anotacion = nueva_capacidad;
    anotacion = new char[capacidad_anotacion];
}

/**
 * @brief Copia una anotación en el atributo interno anotacion.
 *
 * @param nueva_anotacion Cadena C (const char*) a copiar como anotación.
 */
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

/**
 * @brief Devuelve el contenido de la anotación.
 *
 * @return Anotación almacenada como string.
 */
string Reservacion::getAnotacion() const {
    return anotacion ? string(anotacion) : "";
}


/**
 * @brief Establece una nueva anotación.
 *
 * @param nueva_anotacion Cadena string a establecer como nueva anotación.
 */
void Reservacion::setAnotacion(const string& nueva_anotacion) {
    incrementarIteracion();
    size_t longitud = nueva_anotacion.length();
    if (longitud > MAX_CARACTERES) {
        longitud = MAX_CARACTERES;
    }

    reservarEspacio(longitud + 1);
    strncpy(anotacion, nueva_anotacion.c_str(), longitud);
    anotacion[longitud] = '\0';
}

/**
 * @brief Filtra alojamientos disponibles según una fecha y duración dadas.
 *
 * @param candidatos Arreglo de alojamientos posibles.
 * @param nCandidatos Número de alojamientos en el arreglo.
 * @param fechaEntrada Fecha deseada para iniciar la estadía.
 * @param noches Número de noches a reservar.
 * @param reservaciones Arreglo de reservaciones existentes.
 * @param nReservaciones Número de reservaciones existentes.
 * @param nDisponibles Variable de salida con el número de alojamientos disponibles encontrados.
 * @return Puntero a un arreglo dinámico de alojamientos disponibles.
 */
// Implementacion del filtro de disponibilidad
// Alojamiento* Reservacion::filtrarDisponiblesPorFecha(Alojamiento* candidatos,
//                                                      int nCandidatos,
//                                                      const Fecha& fechaEntrada,
//                                                      int noches,
//                                                      const Reservacion* reservaciones,
//                                                      int nReservaciones,
//                                                      int& nDisponibles) {

//     incrementarIteracion();
//     agregarMemoria(sizeof(Alojamiento*) + sizeof(int)*2);

//     Alojamiento* disponibles = new Alojamiento[nCandidatos];
//     nDisponibles = 0;

//     for (int i = 0; i < nCandidatos; ++i) {
//         incrementarIteracion();
//         bool estaDisponible = true;
//         agregarMemoria(sizeof(bool));

//         // Verificar conflictos con reservaciones existentes
//         for (int j = 0; j < nReservaciones && estaDisponible; ++j) {
//             incrementarIteracion();
//             if (reservaciones[j].getCodigoAlojamiento() == candidatos[i].getCodigo()) {
//                 incrementarIteracion();
//                 Fecha fechaReservada(reservaciones[j].getFechaEntrada());
//                 agregarMemoria(sizeof(Fecha));
//                 if (hayConflictoFechas(fechaEntrada, noches,
//                                        fechaReservada, reservaciones[j].getDuracion())) {
//                     estaDisponible = false;
//                 }
//             }
//         }

//         if (estaDisponible) {
//             incrementarIteracion();
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

    incrementarIteracion();
    agregarMemoria(sizeof(Alojamiento*) + sizeof(int)*2);
    Alojamiento* disponibles = new Alojamiento[nCandidatos];
    agregarMemoria(sizeof(Alojamiento) * nCandidatos);
    nDisponibles = 0;

    cout << "\n=== DEBUG: Filtrado por fecha ===" << endl;
    cout << "Fecha buscada: " << fechaEntrada.toString() << " por " << noches << " noches" << endl;
    cout << "Total reservaciones existentes: " << nReservaciones << endl;

    for (int i = 0; i < nCandidatos; ++i) {
        incrementarIteracion();
        cout << "\nEvaluando alojamiento: " << candidatos[i].getCodigo() << endl;
        bool estaDisponible = true;
        agregarMemoria(sizeof(bool));

        for (int j = 0; j < nReservaciones && estaDisponible; ++j) {
            incrementarIteracion();
            if (reservaciones[j].getCodigoAlojamiento() == candidatos[i].getCodigo()) {
                incrementarIteracion();
                Fecha fechaReservada(reservaciones[j].getFechaEntrada());
                agregarMemoria(sizeof(Fecha));
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
            incrementarIteracion();
            disponibles[nDisponibles++] = candidatos[i];
            cout << "ALOJAMIENTO DISPONIBLE" << endl;
        } else {
            incrementarIteracion();
            cout << "ALOJAMIENTO NO DISPONIBLE" << endl;
        }

        agregarMemoria(-static_cast<long long>(sizeof(bool)));
    }

    cout << "Total disponibles: " << nDisponibles << endl;
    return disponibles;
}

/**
 * @brief Determina si hay conflicto entre dos rangos de fechas de entrada y duración.
 *
 * @param fechaEntrada1 Fecha de entrada de la primera reservación.
 * @param noches1 Número de noches de la primera reservación.
 * @param fechaEntrada2 Fecha de entrada de la segunda reservación.
 * @param noches2 Número de noches de la segunda reservación.
 * @return true Si hay conflicto (se solapan fechas).
 * @return false Si no hay conflicto.
 */
// bool Reservacion::hayConflictoFechas(const Fecha& fechaEntrada1, int noches1,
//                                      const Fecha& fechaEntrada2, int noches2) {
//     // Medición de recursos
//     incrementarIteracion();
//     agregarMemoria(sizeof(int)*4);

//     incrementarIteracion();
//     int inicio1 = fechaEntrada1.aDiasDesdeFechaBase();
//     incrementarIteracion();
//     int inicio2 = fechaEntrada2.aDiasDesdeFechaBase();

//     int fin1 = inicio1 + noches1 - 1;
//     int fin2 = inicio2 + noches2 - 1;
//     incrementarIteracion();

//     bool resultado = !(fin1 < inicio2 || fin2 < inicio1);
//     incrementarIteracion();
//     agregarMemoria(-static_cast<long long>(sizeof(int)*4));

//     return resultado;
// }

bool Reservacion::hayConflictoFechas(const Fecha& fechaEntrada1, int noches1,
                                     const Fecha& fechaEntrada2, int noches2) {
    incrementarIteracion();
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
    incrementarIteracion();
    agregarMemoria(sizeof(bool));

    cout << "Hay conflicto? " << (conflicto ? "Si" : "No") << endl;
    agregarMemoria(-static_cast<long long>(sizeof(int)*4 + sizeof(bool)));

    return conflicto;
}

/**
 * @brief Muestra los detalles de la reservación, diferenciando si el usuario es anfitrión o huésped.
 *
 * @param numero Número de la reservación para mostrar.
 * @param esParaAnfitrion Indica si la vista es para anfitrión (true) o huésped (false).
 * @param sistema Puntero al sistema para acceder a datos del huésped si es necesario.
 */
void Reservacion::mostrarDetalle(int numero, bool esParaAnfitrion, Sistema* sistema) const {

    incrementarIteracion();
    agregarMemoria(sizeof(Fecha)*2 + sizeof(bool) + sizeof(Sistema*));

    Fecha fechaEntrada(this->getFechaEntrada());
    Fecha fechaFinal = fechaEntrada.sumarDias(this->getDuracion());
    incrementarIteracion();

    cout << "--- RESERVACION " << numero << " ---\n";
    cout << "Codigo: " << this->getCodigo() << "\n";
    incrementarIteracion();

    if (esParaAnfitrion) {
        incrementarIteracion();
        // Vista para anfitrion: incluye datos del huesped
        cout << "Alojamiento: " << this->getCodigoAlojamiento() << "\n";
        cout << "Huesped: " << obtenerNombreHuesped(sistema) << "\n";
        cout << "Documento Huesped: " << this->getDocumento() << "\n";
        incrementarIteracion();
    } else {
        incrementarIteracion();
        // Vista para huesped: incluye estado y metodo de pago
        cout << "Alojamiento: " << this->getCodigoAlojamiento() << "\n";
        cout << "Estado: " << (fechaFinal.yaPaso() ? "COMPLETADA" : "PENDIENTE") << "\n";
        cout << "Metodo de Pago: ";
        incrementarIteracion();

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
        incrementarIteracion();
    }

    cout << "Fecha Entrada: " << fechaEntrada.toString() << "\n";
    cout << "Fecha Salida: " << fechaFinal.toString() << "\n";
    cout << "Duracion: " << this->getDuracion() << " noches\n";
    cout << "Monto: $" << fixed << setprecision(0) << this->getMonto() << "\n";

    string anotacion = this->getAnotacion();
    if (!anotacion.empty()) {
        cout << "Anotaciones: " << anotacion << "\n";
        incrementarIteracion();
    }
    cout << "--------------------------------\n";
}

/**
 * @brief Obtiene el nombre del huésped asociado a la reservación.
 *
 * Busca dentro del sistema el huésped cuyo documento coincida con el de esta reservación.
 * También registra el uso de memoria y el número de iteraciones para efectos de análisis de recursos.
 *
 * @param sistema Puntero al sistema que contiene el arreglo de huéspedes y su cantidad.
 * @return Nombre del huésped como string si se encuentra; "No encontrado" en caso contrario.
 */
string Reservacion::obtenerNombreHuesped(Sistema* sistema) const {
    incrementarIteracion();
    agregarMemoria(sizeof(Huesped*) + sizeof(int));
    const Huesped* huespedes = sistema->getHuespedes();
    int totalHuespedes = sistema->getNumHuespedes();

    for (int i = 0; i < totalHuespedes; i++) {
        incrementarIteracion();
        if (huespedes[i].getDocumento() == this->getDocumento()) {
            return huespedes[i].getNombre();
        }
    }
    return "No encontrado";
}
