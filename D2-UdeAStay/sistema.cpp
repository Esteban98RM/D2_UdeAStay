#include "sistema.h"
#include "anfitrion.h"
#include "huesped.h"
#include "gestionarchivos.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Constructores
Sistema::Sistema() : documento(""), anfitriones(nullptr), huespedes(nullptr),
    alojamientos(nullptr), reservaciones(nullptr), numAnfitriones(0),
    numHuespedes(0), numAlojamientos(0), numReservaciones(0) {}

Sistema::Sistema(const string& doc) : Sistema() {
    documento = doc;
}

Sistema::Sistema(const string& doc, Anfitrion* anf, Huesped* hue) : Sistema(doc) {
    // Implementacion para manejar los punteros a anfitriones y huespedes
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

// Autenticacion
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

//Iniciar Sesion segun ROL
void Sistema::iniciarSesion(const string &documento, const string &clave) {
    string tipoRol;

    if (autenticarUsuario(documento, clave, tipoRol)) {
        cout << "\nBienvenido " << tipoRol << "!\n";

        this->documento = documento;

        if (tipoRol == "Anfitrion") {
            for (int i = 0; i < numAnfitriones; i++) {
                if (anfitriones[i].getDocumento() == documento) {
                    mostrarMenuAnfitrion(anfitriones[i]);
                    break;
                }
            }
        } else if (tipoRol == "Huesped") {
            for (int i = 0; i < numHuespedes; i++) {
                if (huespedes[i].getDocumento() == documento) {
                    mostrarMenuHuesped(huespedes[i]);
                    break;
                }
            }
        }
    } else {
        cout << "\nCredenciales incorrectas\n";
    }
}

//Menu de Anfitrion
void Sistema::mostrarMenuAnfitrion(Anfitrion& anfitrion) {
    int opcion;
    do {
        cout << "\n--- MENU ANFITRION ---\n";
        cout << "2. Ver mis alojamientos\n";
        cout << "3. Ver reservaciones de mis alojamientos\n";
        cout << "4. Actualizar informacion de alojamiento\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

    //     switch(opcion) {
    //     case 1:
    //     case 2:
    //         anfitrion.mostrarAlojamientos(this);
    //         break;
    //     case 3:
    //         anfitrion.mostrarReservacionesAlojamientos(this);
    //         break;
    //     case 4:
    //         anfitrion.actualizarAlojamiento(this);
    //         break;
    //     case 5:
    //         cout << "Saliendo del sistema...\n";
    //         break;
    //     default:
    //         cout << "Opción no valida\n";
    //     }
    } while (opcion != 5);
}

//Menu de Huesped
void Sistema::mostrarMenuHuesped(Huesped& huesped) {
    int opcion;
    do {
        cout << "\n--- MENU HUESPED ---\n";
        cout << "1. Buscar y reservar alojamiento\n";
        cout << "2. Ver mis reservaciones\n";
        cout << "3. Cancelar reservacion\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch(opcion) {
        case 1:
           huesped.ReservarAlojamiento(this);
            break;
        // case 2:
        //     huesped.mostrarReservaciones();
        //     break;
        // case 3:
        //     huesped.cancelarReservacion(this);
        //     break;
        // case 4:
        //     cout << "Saliendo del sistema...\n";
        //     break;
        // default:
        //     cout << "Opcion no valida\n";
        }
    } while (opcion != 4);
}

// Alojamiento* Sistema::filtrarAlojamientosDisponibles(
//     const string& municipio, float precioMax, float puntuacionMin, int& cantidadFiltrada
//     ) {
//     Alojamiento* resultado = new Alojamiento[numAlojamientos];
//     cantidadFiltrada = 0;

//     for (int i = 0; i < numAlojamientos; ++i) {
//         if (alojamientos[i].getMunicipio() != municipio)
//             continue;

//         if (precioMax >= 0 && alojamientos[i].getPrecio() > precioMax)
//             continue;

//         string docAnfitrion = alojamientos[i].getDocumentoAnfitrion();
//         float puntuacion = -1;

//         for (int j = 0; j < numAnfitriones; ++j) {
//             if (anfitriones[j].getDocumento() == docAnfitrion) {
//                 puntuacion = anfitriones[j].getPuntuacion();
//                 break;
//             }
//         }

//         if (puntuacionMin >= 0 && puntuacion < puntuacionMin)
//             continue;

//         resultado[cantidadFiltrada++] = alojamientos[i];
//     }

//     return resultado;
// }

Alojamiento* Sistema::filtrarAlojamientosDisponibles(
    const string& municipio, float precioMax, float puntuacionMin, int& cantidadFiltrada
    ) {
    Alojamiento* resultado = new Alojamiento[numAlojamientos];
    cantidadFiltrada = 0;

    cout << "\n=== DEBUG FILTRADO ===" << endl;
    cout << "Municipio buscado: " << municipio << endl;
    cout << "Precio max: " << precioMax << endl;
    cout << "Puntuacion min: " << puntuacionMin << endl;

    for (int i = 0; i < numAlojamientos; ++i) {
        cout << "\nEvaluando alojamiento " << i << ":" << endl;
        cout << "Municipio: " << alojamientos[i].getMunicipio() << endl;
        cout << "Precio: " << alojamientos[i].getPrecio() << endl;

        if (alojamientos[i].getMunicipio() != municipio) {
            cout << " - No coincide municipio" << endl;
            continue;
        }

        if (precioMax >= 0 && alojamientos[i].getPrecio() > precioMax) {
            cout << " - Excede precio maximo" << endl;
            continue;
        }

        string docAnfitrion = alojamientos[i].getDocumentoAnfitrion();
        float puntuacion = -1;

        for (int j = 0; j < numAnfitriones; ++j) {
            if (anfitriones[j].getDocumento() == docAnfitrion) {
                puntuacion = anfitriones[j].getPuntuacion();
                break;
            }
        }

        cout << "Puntuacion anfitrion: " << puntuacion << endl;

        if (puntuacionMin >= 0 && puntuacion < puntuacionMin) {
            cout << " - No cumple puntuacion minima" << endl;
            continue;
        }

        resultado[cantidadFiltrada++] = alojamientos[i];
        cout << " - ACEPTADO" << endl;
    }

    cout << "Total encontrados: " << cantidadFiltrada << endl;
    return resultado;
}

// Implementación de Sistema
Alojamiento* Sistema::buscarAlojamientosDisponibles(const string& municipio,
                                                    const Fecha& fechaEntrada,
                                                    int noches,
                                                    float precioMax,
                                                    float puntuacionMin,
                                                    int& cantidadEncontrada) {
    // 1. Filtrar por criterios básicos
    int nFiltrados = 0;
    Alojamiento* candidatos = filtrarAlojamientosDisponibles(
        municipio, precioMax, puntuacionMin, nFiltrados
        );

    if (nFiltrados == 0) {
        cantidadEncontrada = 0;
        return nullptr;
    }

    // 2. Filtrar por disponibilidad de fechas
    Alojamiento* disponibles = Reservacion::filtrarDisponiblesPorFecha(
        candidatos, nFiltrados,
        fechaEntrada, noches,
        getReservaciones(), getNumReservaciones(),
        cantidadEncontrada
        );

    delete[] candidatos;
    return disponibles;
}

Reservacion* Sistema::crearReservacion(const Alojamiento& alojamiento,
                                       const Fecha& fechaEntrada,
                                       int noches,
                                       Huesped* huesped) {
    // Validar disponibilidad del huésped
    if (!validarDisponibilidadHuesped(huesped->getDocumento(), fechaEntrada, noches)) {
        cout << "Error: Ya tienes una reservación que se superpone con estas fechas.\n";
        return nullptr;
    }

    // Generar código único
    string codigo = generarCodigoReservacion();

    // Calcular monto total
    float montoTotal = alojamiento.getPrecio() * noches;

    // Solicitar datos adicionales al huésped
    char metodoPago = huesped->solicitarMetodoPago();
    string anotaciones = huesped->solicitarAnotaciones();
    string fechaPago = Fecha::obtenerFechaActual();

    // Crear reservación
    Reservacion* nuevaReservacion = new Reservacion(
        codigo,
        fechaEntrada.toString(),
        noches,
        alojamiento.getCodigo(),
        huesped->getDocumento(),
        metodoPago,
        fechaPago,
        montoTotal,
        anotaciones
        );

    // Agregar al sistema
    agregarReservacion(*nuevaReservacion);

    return nuevaReservacion;
}

string Sistema::generarCodigoReservacion() {
    // Generar número aleatorio de 4 dígitos
    srand(time(0));
    int numeroAleatorio = rand() % 9000 + 1000; // Entre 1000 y 9999

    // Generar letra aleatoria
    char letra = 'A' + (rand() % 26);

    ostringstream oss;
    oss << "RES-" << setfill('0') << setw(3) << (getNumReservaciones() + 1)
        << "-" << numeroAleatorio << letra;

    return oss.str();
}

Alojamiento* Sistema::buscarAlojamientoPorCodigo(const string& codigo) {
    for (int i = 0; i < getNumAlojamientos(); i++) {
        if (getAlojamientos()[i].getCodigo() == codigo) {
            return &getAlojamientos()[i];
        }
    }
    return nullptr;
}

bool Sistema::validarDisponibilidadHuesped(const string& documentoHuesped,
                                           const Fecha& fechaEntrada,
                                           int noches) {
    const Reservacion* reservaciones = getReservaciones();
    int numReservaciones = getNumReservaciones();

    for (int i = 0; i < numReservaciones; i++) {
        if (reservaciones[i].getDocumento() == documentoHuesped) {
            Fecha fechaReservada(reservaciones[i].getFechaEntrada());
            if (Reservacion::hayConflictoFechas(fechaEntrada, noches,
                                                fechaReservada, reservaciones[i].getDuracion())) {
                return false; // Hay conflicto
            }
        }
    }
    return true; // No hay conflictos
}

// Implementación de agregarReservacion
void Sistema::agregarReservacion(const Reservacion& reservacion) {
    // Expandir array si es necesario
    Reservacion* nuevasReservaciones = new Reservacion[numReservaciones + 1];

    // Copiar reservaciones existentes
    for (int i = 0; i < numReservaciones; i++) {
        nuevasReservaciones[i] = reservaciones[i];
    }

    // Agregar nueva reservación
    nuevasReservaciones[numReservaciones] = reservacion;

    // Actualizar array principal
    delete[] reservaciones;
    reservaciones = nuevasReservaciones;
    numReservaciones++;

    // Guardar en archivo
    gestionArchivos->guardarReservacion(reservacion);
}

// Liberacion de memoria
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
