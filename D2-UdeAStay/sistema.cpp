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
    numHuespedes(0), numAlojamientos(0), numReservaciones(0), gestionArchivos(new GestionArchivos()) {
    ultimoIdReservacion = gestionArchivos->cargarUltimoIdReservacion();
}

Sistema::Sistema(const string& doc) : Sistema() {
    documento = doc;
}

// Sistema::Sistema(const string& doc, Anfitrion* anf, Huesped* hue) : Sistema(doc) {
//     // Implementacion para manejar los punteros a anfitriones y huespedes
// }

// Destructor
Sistema::~Sistema() {

    delete gestionArchivos;

    if (anfitriones) delete[] anfitriones;
    if (huespedes) delete[] huespedes;
    if (alojamientos) delete[] alojamientos;
    if (reservaciones) delete[] reservaciones;

    numAnfitriones = numHuespedes = numAlojamientos = numReservaciones = 0;
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

// bool Sistema::autenticarUsuario(const string& doc, const string& clave, string& tipoRol) {
//     cout << "[DEBUG] Autenticando usuario: doc=" << doc << ", clave=" << clave << endl;

//     // Buscar en anfitriones
//     for (int i = 0; i < numAnfitriones; i++) {
//         cout << "[DEBUG] Comparando con Anfitrion #" << i
//              << " | doc=" << anfitriones[i].getDocumento()
//              << ", clave=" << anfitriones[i].getClave() << endl;

//         if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == clave) {
//             tipoRol = "Anfitrion";
//             cout << "[DEBUG] Coincidencia encontrada como ANFITRION!" << endl;
//             return true;
//         }
//     }

//     // Buscar en huespedes
//     for (int i = 0; i < numHuespedes; i++) {
//         cout << "[DEBUG] Comparando con Huesped #" << i
//              << " | doc=" << huespedes[i].getDocumento()
//              << ", clave=" << huespedes[i].getClave() << endl;

//         if (huespedes[i].getDocumento() == doc && huespedes[i].getClave() == clave) {
//             tipoRol = "Huesped";
//             cout << "[DEBUG] Coincidencia encontrada como HUESPED!" << endl;
//             return true;
//         }
//     }

//     cout << "[DEBUG] No se encontro coincidencia en anfitriones ni huespedes." << endl;
//     return false; // No encontrado
// }

//Iniciar Sesion segun ROL
bool Sistema::iniciarSesion(const string &documento, const string &clave) {
    string tipoRol;

    if (autenticarUsuario(documento, clave, tipoRol)) {
        cout << "\nInicio de sesion exitosa su ROL es: " << tipoRol << endl;
        this->documento = documento;

        if (tipoRol == "Anfitrion") {
            for (int i = 0; i < numAnfitriones; i++) {
                if (anfitriones[i].getDocumento() == documento) {
                    cout << "\nBienvenido Anfitrion: " << anfitriones[i].getCodigo() << endl;
                    mostrarMenuAnfitrion(anfitriones[i]);
                    break;
                }
            }
        } else if (tipoRol == "Huesped") {
            for (int i = 0; i < numHuespedes; i++) {
                if (huespedes[i].getDocumento() == documento) {
                    cout << "\nBienvenido Huesped: " << huespedes[i].getNombre() << endl;
                    mostrarMenuHuesped(huespedes[i]);
                    break;
                }
            }
        }
        return true;
    }

    return false;
}

//Menu de Anfitrion
void Sistema::mostrarMenuAnfitrion(Anfitrion& anfitrion) {
    int opcion;
    bool volverLogin = false;

    while (!volverLogin) {
        cout << "\n--- MENU ANFITRION ---\n";
        cout << "1. Ver reservaciones activas\n";
        cout << "2. Cancelar reservaciones\n";
        cout << "3. Actualizar historico de las reservaciones\n";
        cout << "4. Cerrar sesion\n";
        cout << "5. Salir del programa\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            anfitrion.mostrarReservacionesActivas(this);
            break;
        case 2:
            anfitrion.cancelarReservacion(this);
            break;
        case 3:
            anfitrion.actualizarHistorico(this);
            break;
        case 4:
            cout << "Cerrando sesion...\n";
            volverLogin = true;
            break;
        case 5:
            cout << "Saliendo del programa...\n";
            exit(0);
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }
}

//Menu de Huesped
void Sistema::mostrarMenuHuesped(Huesped& huesped) {
    int opcion;
    bool volverLogin = false;

    while (!volverLogin) {
        cout << "\n--- MENU HUESPED ---\n";
        cout << "1. Buscar y reservar alojamiento\n";
        cout << "2. Ver mis reservaciones\n";
        cout << "3. Cancelar reservacion\n";
        cout << "4. Cerrar sesion\n";
        cout << "5. Salir del programa\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            huesped.ReservarAlojamiento(this);
            break;
        case 2:
            huesped.mostrarReservaciones(this);
            break;
        case 3:
            huesped.anularReservacion(this);
            break;
        case 4:
            cout << "Cerrando sesion...\n";
            volverLogin = true;
            break;
        case 5:
            cout << "Saliendo del programa...\n";
            exit(0);
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }
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

// Implementacion de Sistema
Alojamiento* Sistema::buscarAlojamientosDisponibles(const string& municipio,
                                                    const Fecha& fechaEntrada,
                                                    int noches,
                                                    float precioMax,
                                                    float puntuacionMin,
                                                    int& cantidadEncontrada) {
    // 1. Filtrar por criterios basicos
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
    // Validar disponibilidad del huesped
    if (!validarDisponibilidadHuesped(huesped->getDocumento(), fechaEntrada, noches)) {
        cout << "Error: Ya tienes una reservacion que se superpone con estas fechas.\n";
        return nullptr;
    }

    // Generar codigo unico
    string codigo = generarCodigoReservacion();

    // Calcular monto total
    float montoTotal = alojamiento.getPrecio() * noches;

    // Solicitar datos adicionales al huesped
    char metodoPago = huesped->solicitarMetodoPago();
    string anotaciones = huesped->solicitarAnotaciones();
    string fechaPago = Fecha::obtenerFechaActual();

    // Crear reservacion
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
    // Incrementar y guardar el ID historico
    ultimoIdReservacion++;
    gestionArchivos->guardarUltimoIdReservacion(ultimoIdReservacion);

    // Generar sufijo aleatorio
    int sufijoNum = rand() % 9000 + 1000;  // Numero entre 1000-9999
    char sufijoLetra = 'A' + (rand() % 26); // Letra entre A-Z

    // Construir el codigo completo
    ostringstream oss;
    oss << "RES-" << ultimoIdReservacion << "-" << sufijoNum << sufijoLetra;

    return oss.str();
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

// Implementacion de agregarReservacion
void Sistema::agregarReservacion(const Reservacion& reservacion) {
    try {
        // 1. Crear nuevo array mas grande
        Reservacion* nuevasReservaciones = new Reservacion[numReservaciones + 1];

        // 2. Copiar reservaciones existentes
        for (int i = 0; i < numReservaciones; i++) {
            nuevasReservaciones[i] = reservaciones[i];
        }

        // 3. Agregar la nueva reservacion
        nuevasReservaciones[numReservaciones] = reservacion;

        // 4. Reemplazar el array antiguo
        delete[] reservaciones;
        reservaciones = nuevasReservaciones;
        numReservaciones++;

        // 5. Guardar en archivo (manejar error aparte para no perder datos)
        try {
            gestionArchivos->guardarReservacion(reservacion);
        } catch (...) {
            cerr << "Error al guardar en archivo (pero se agrego en memoria)\n";
        }

    } catch (...) {
        cerr << "Error al agregar reservacion (memoria insuficiente)\n";
        throw;
    }
}

bool Sistema::eliminarReservacionPorCodigo(const string& codigo, const string& documento) {
    for (int i = 0; i < numReservaciones; ++i) {
        if (reservaciones[i].getCodigo() == codigo &&
            reservaciones[i].getDocumento() == documento) {

            // Reorganizar el arreglo (no se usa delete porque son objetos, no punteros)
            for (int j = i; j < numReservaciones - 1; ++j) {
                reservaciones[j] = reservaciones[j + 1];
            }

            numReservaciones--;

            // Actualizar archivo inmediatamente
            gestionArchivos->actualizarArchivoReservaciones(reservaciones, numReservaciones);

            return true;
        }
    }
    return false;
}

// Sobrecarga para huesped: obtiene todas sus reservaciones clasificadas
Reservacion* Sistema::obtenerReservaciones(const string& documentoHuesped,
                                           int& totalFuturas,
                                           int& totalPasadas) const {
    const Reservacion* todasReservaciones = this->getReservaciones();
    int totalReservaciones = this->getNumReservaciones();

    // Primera pasada: contar reservaciones del huésped
    int contadorTotal = 0;
    for (int i = 0; i < totalReservaciones; i++) {
        if (todasReservaciones[i].getDocumento() == documentoHuesped) {
            contadorTotal++;
        }
    }

    if (contadorTotal == 0) {
        totalFuturas = 0;
        totalPasadas = 0;
        return nullptr;
    }

    // Segunda pasada: clasificar reservaciones
    totalFuturas = 0;
    totalPasadas = 0;

    bool* esPasada = new bool[contadorTotal];
    int index = 0;

    for (int i = 0; i < totalReservaciones; i++) {
        if (todasReservaciones[i].getDocumento() == documentoHuesped) {
            Fecha fechaEntrada(todasReservaciones[i].getFechaEntrada());
            Fecha fechaFinal = fechaEntrada.sumarDias(todasReservaciones[i].getDuracion());

            esPasada[index] = fechaFinal.yaPaso();
            if (esPasada[index]) {
                totalPasadas++;
            } else {
                totalFuturas++;
            }
            index++;
        }
    }

    // Crear arreglo resultado (futuras primero, luego pasadas)
    Reservacion* resultado = new Reservacion[contadorTotal];
    int idxFuturas = 0;
    int idxPasadas = totalFuturas;
    index = 0;

    for (int i = 0; i < totalReservaciones; i++) {
        if (todasReservaciones[i].getDocumento() == documentoHuesped) {
            if (!esPasada[index]) {
                resultado[idxFuturas++] = todasReservaciones[i];
            } else {
                resultado[idxPasadas++] = todasReservaciones[i];
            }
            index++;
        }
    }

    delete[] esPasada;
    return resultado;
}

// Sobrecarga para anfitrion: obtiene reservaciones activas en rango de fechas
Reservacion* Sistema::obtenerReservaciones(const string& documentoAnfitrion,
                                           const Fecha& fechaInicio,
                                           const Fecha& fechaFin,
                                           int& totalEncontradas) const {
    const Reservacion* todasReservaciones = this->getReservaciones();
    int totalReservaciones = this->getNumReservaciones();
    const Alojamiento* alojamientos = this->getAlojamientos();
    int totalAlojamientos = this->getNumAlojamientos();

    // Primera pasada: contar reservaciones que cumplen criterios
    totalEncontradas = 0;
    for (int i = 0; i < totalReservaciones; i++) {
        if (esReservacionDelAnfitrionEnRango(todasReservaciones[i], documentoAnfitrion,
                                             fechaInicio, fechaFin, alojamientos, totalAlojamientos)) {
            totalEncontradas++;
        }
    }

    if (totalEncontradas == 0) {
        return nullptr;
    }

    // Segunda pasada: llenar arreglo resultado
    Reservacion* resultado = new Reservacion[totalEncontradas];
    int index = 0;

    for (int i = 0; i < totalReservaciones; i++) {
        if (esReservacionDelAnfitrionEnRango(todasReservaciones[i], documentoAnfitrion,
                                             fechaInicio, fechaFin, alojamientos, totalAlojamientos)) {
            resultado[index++] = todasReservaciones[i];
        }
    }

    return resultado;
}

// Metodo auxiliar privado para validar reservaciones del anfitrion
bool Sistema::esReservacionDelAnfitrionEnRango(const Reservacion& reserva,
                                               const string& documentoAnfitrion,
                                               const Fecha& fechaInicio,
                                               const Fecha& fechaFin,
                                               const Alojamiento* alojamientos,
                                               int totalAlojamientos) const {
    // Verificar si la reservacion corresponde a un alojamiento del anfitrion
    bool esDelAnfitrion = false;
    for (int i = 0; i < totalAlojamientos; i++) {
        if (alojamientos[i].getCodigo() == reserva.getCodigoAlojamiento() &&
            alojamientos[i].getDocumentoAnfitrion() == documentoAnfitrion) {
            esDelAnfitrion = true;
            break;
        }
    }

    if (!esDelAnfitrion) {
        return false;
    }

    // Verificar si esta en el rango de fechas y es activa
    Fecha fechaEntrada(reserva.getFechaEntrada());
    Fecha fechaSalida = fechaEntrada.sumarDias(reserva.getDuracion());

    // Reservacion activa: no ha terminado aun y esta en el rango
    return !fechaSalida.yaPaso() &&
           fechaEntrada >= fechaInicio &&
           fechaEntrada <= fechaFin;
}

string Sistema::obtenerUltimaFechaCorteHistorico() {
    return gestionArchivos->cargarUltimaFechaCorteHistorico();
}

void Sistema::mostrarEstadisticasHistorico(const Fecha& fechaCorte, int movidas, int activas) {
    cout << "\n=== RESUMEN DE ACTUALIZACION ===\n";
    cout << "Fecha de corte: " << fechaCorte.toString() << endl;
    cout << "Reservaciones movidas al historico: " << movidas << endl;
    cout << "Reservaciones activas restantes: " << activas << endl;

    // Calcular fecha límite para nuevas reservaciones (12 meses desde fecha de corte)
    Fecha fechaLimite = fechaCorte.sumarDias(365); // Aproximadamente 12 meses
    cout << "Nuevas reservaciones habilitadas hasta: " << fechaLimite.toString() << endl;
    cout << "================================\n";
}

void Sistema::procesarActualizacionHistorico(const Fecha& fechaCorte) {
    cout << "\n=== PROCESANDO ACTUALIZACION DE HISTORICO ===\n";
    cout << "LOGICA: Solo reservaciones YA FINALIZADAS van al historico.\n";
    cout << "La fecha de corte (" << fechaCorte.toString()
         << ") establece la nueva base para proximas reservaciones.\n\n";

    Fecha fechaHoy(Fecha::obtenerFechaActual());

    // Contadores para estadisticas detalladas
    int reservacionesFinalizadas = 0;    // Ya pasaron completamente
    int reservacionesActivas = 0;        // Futuras o en curso
    int reservacionesEnCurso = 0;        // Iniciadas pero no terminadas

    // Primera pasada: clasificar reservaciones por estado
    for (int i = 0; i < numReservaciones; i++) {
        Fecha fechaInicio(reservaciones[i].getFechaEntrada());
        Fecha fechaFin = fechaInicio.calcularFechaFinal(reservaciones[i].getDuracion());

        if (fechaFin < fechaHoy) {
            // La estadia ya termino completamente
            reservacionesFinalizadas++;
        } else if (fechaInicio <= fechaHoy && fechaFin >= fechaHoy) {
            // Reservacion en curso (huesped actualmente hospedado)
            reservacionesEnCurso++;
            reservacionesActivas++;
        } else {
            // Reservacion futura
            reservacionesActivas++;
        }
    }

    // Mostrar analisis detallado
    cout << "ANALISIS DE RESERVACIONES:\n";
    cout << "- Reservaciones FINALIZADAS (van al historico): " << reservacionesFinalizadas << endl;
    cout << "- Reservaciones EN CURSO (se mantienen activas): " << reservacionesEnCurso << endl;
    cout << "- Reservaciones FUTURAS (se mantienen activas): " << (reservacionesActivas - reservacionesEnCurso) << endl;
    cout << "- Total que permanecen activas: " << reservacionesActivas << endl;

    if (reservacionesFinalizadas == 0) {
        cout << "\nNo hay reservaciones finalizadas para mover al historico.\n";
        // But still update the base date for new reservations
        cout << "Se actualiza la fecha base para habilitar reservaciones hasta: "
             << fechaCorte.sumarDias(365).toString() << endl;

        // Guardar la nueva fecha de corte como referencia
        gestionArchivos->guardarUltimaFechaCorteHistorico(fechaCorte.toString());
        cout << "Sistema actualizado con nueva fecha base.\n";
        return;
    }

    // Confirmar operacion
    cout << "\nProceder a mover " << reservacionesFinalizadas
         << " reservaciones finalizadas al historico? (s/n): ";
    char confirmar;
    cin >> confirmar;

    if (confirmar != 's' && confirmar != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    // Crear arrays para separar reservaciones
    Reservacion* reservacionesParaHistorico = new Reservacion[reservacionesFinalizadas];
    Reservacion* reservacionesParaMantener = new Reservacion[reservacionesActivas];

    int indiceHistorico = 0;
    int indiceMantener = 0;

    // Separar reservaciones basado en si YA TERMINARON
    for (int i = 0; i < numReservaciones; i++) {
        Fecha fechaInicio(reservaciones[i].getFechaEntrada());
        Fecha fechaFin = fechaInicio.calcularFechaFinal(reservaciones[i].getDuracion());

        if (fechaFin < fechaHoy) {
            // Ya termino completamente, va al historico
            reservacionesParaHistorico[indiceHistorico] = reservaciones[i];
            indiceHistorico++;
        } else {
            // Activa, en curso o futura, se mantiene
            reservacionesParaMantener[indiceMantener] = reservaciones[i];
            indiceMantener++;
        }
    }

    // Guardar reservaciones finalizadas en historico
    if (gestionArchivos->guardarReservacionesHistorico(reservacionesParaHistorico, reservacionesFinalizadas, fechaCorte.toString())) {
        cout << reservacionesFinalizadas << " reservaciones finalizadas movidas al historico.\n";

        // Actualizar estructura de datos actual
        delete[] reservaciones;
        reservaciones = reservacionesParaMantener;
        numReservaciones = reservacionesActivas;

        // Actualizar archivo principal de reservaciones
        gestionArchivos->actualizarArchivoReservaciones(reservaciones, numReservaciones);

        // Guardar la nueva fecha de corte
        gestionArchivos->guardarUltimaFechaCorteHistorico(fechaCorte.toString());

        cout << "Archivo de reservaciones actualizado.\n";
        cout << "Quedan " << reservacionesActivas << " reservaciones activas.\n";
        cout << "Nueva fecha de corte guardada: " << fechaCorte.toString() << "\n";

        // Mostrar estadisticas
        mostrarEstadisticasHistorico(fechaCorte, reservacionesFinalizadas, reservacionesActivas);

    } else {
        cout << "Error: No se pudo actualizar el historico.\n";
        delete[] reservacionesParaMantener;
    }
    delete[] reservacionesParaHistorico;
}
