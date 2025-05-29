#include "sistema.h"
#include "anfitrion.h"
#include "huesped.h"
#include "gestionarchivos.h"
#include "medicionrecursos.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Constructores
Sistema::Sistema() : documento(""), anfitriones(nullptr), huespedes(nullptr),
    alojamientos(nullptr), reservaciones(nullptr), numAnfitriones(0),
    numHuespedes(0), numAlojamientos(0), numReservaciones(0), gestionArchivos(new GestionArchivos()) {

    incrementarIteracion();
    agregarMemoria(sizeof(Sistema));
    agregarMemoria(sizeof(GestionArchivos));

    ultimoIdReservacion = gestionArchivos->cargarUltimoIdReservacion();
    incrementarIteracion();
}

Sistema::Sistema(const string& doc) : Sistema() {
    documento = doc;
}

// Destructor
Sistema::~Sistema() {
    incrementarIteracion();

    delete gestionArchivos;
    agregarMemoria(-static_cast<long long>(sizeof(GestionArchivos)));

    // Liberar arreglos dinámicos
    if (anfitriones) {
        agregarMemoria(-static_cast<long long>(sizeof(Anfitrion) * numAnfitriones));
        delete[] anfitriones;
    }
    if (huespedes) {
        agregarMemoria(-static_cast<long long>(sizeof(Huesped) * numHuespedes));
        delete[] huespedes;
    }
    if (alojamientos) {
        agregarMemoria(-static_cast<long long>(sizeof(Alojamiento) * numAlojamientos));
        delete[] alojamientos;
    }
    if (reservaciones) {
        agregarMemoria(-static_cast<long long>(sizeof(Reservacion) * numReservaciones));
        delete[] reservaciones;
    }

    numAnfitriones = numHuespedes = numAlojamientos = numReservaciones = 0;
}

// Operador de comparacion
bool Sistema::operator==(const Sistema& otro) const {
    incrementarIteracion(); // Por la comparación

    // Comparación de strings puede ser costosa
    bool resultado = documento == otro.documento;
    incrementarIteracion(); // Por la operación de comparación

    return resultado;
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
        incrementarIteracion();
        if (anfitriones[i].getDocumento() == doc && anfitriones[i].getClave() == clave) {
            tipoRol = "Anfitrion";
            return true;
        }
    }

    // Buscar en huespedes
    for (int i = 0; i < numHuespedes; i++) {
        incrementarIteracion();
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
//         incrementarIteracion();
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
//         incrementarIteracion();
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
                incrementarIteracion();
                if (anfitriones[i].getDocumento() == documento) {
                    cout << "\nBienvenido Anfitrion: " << anfitriones[i].getCodigo() << endl;
                    mostrarMenuAnfitrion(anfitriones[i]);
                    break;
                }
            }
        } else if (tipoRol == "Huesped") {
            for (int i = 0; i < numHuespedes; i++) {
                incrementarIteracion();
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
            incrementarIteracion(); // Ver reservaciones activas
            anfitrion.mostrarReservacionesActivas(this);
            break;
        case 2:
            incrementarIteracion(); // Cancelar reservaciones
            anfitrion.cancelarReservacion(this);
            break;
        case 3:
            incrementarIteracion(); // Actualizar histórico
            anfitrion.actualizarHistorico(this);
            break;
        case 4:
            mostrarEstadisticasRecursos();
            cout << "Cerrando sesion...\n";
            volverLogin = true;
            break;
        case 5:
            mostrarEstadisticasRecursos();
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
            incrementarIteracion(); // Buscar y reservar alojamiento
            huesped.ReservarAlojamiento(this);
            break;
        case 2:
            incrementarIteracion(); // Ver mis reservaciones
            huesped.mostrarReservaciones(this);
            break;
        case 3:
            incrementarIteracion(); // Cancelar reservacion
            huesped.anularReservacion(this);
            break;
        case 4:
            mostrarEstadisticasRecursos();
            cout << "Cerrando sesion...\n";
            volverLogin = true;
            break;
        case 5:
            mostrarEstadisticasRecursos();
            cout << "Saliendo del programa...\n";
            exit(0);
            break;
        default:
            cout << "Opcion no valida. Intente nuevamente.\n";
        }
    }
}

Alojamiento* Sistema::filtrarAlojamientosDisponibles(
    const string& municipio, float precioMax, float puntuacionMin, int& cantidadFiltrada
    ) {
    Alojamiento* resultado = new Alojamiento[numAlojamientos];
    agregarMemoria(sizeof(Alojamiento) * numAlojamientos);
    cantidadFiltrada = 0;

    for (int i = 0; i < numAlojamientos; ++i) {
        incrementarIteracion();
        if (alojamientos[i].getMunicipio() != municipio)
            continue;

        if (precioMax >= 0 && alojamientos[i].getPrecio() > precioMax)
            continue;

        string docAnfitrion = alojamientos[i].getDocumentoAnfitrion();
        float puntuacion = -1;

        for (int j = 0; j < numAnfitriones; ++j) {
            incrementarIteracion();
            if (anfitriones[j].getDocumento() == docAnfitrion) {
                puntuacion = anfitriones[j].getPuntuacion();
                break;
            }
        }

        if (puntuacionMin >= 0 && puntuacion < puntuacionMin)
            continue;

        resultado[cantidadFiltrada++] = alojamientos[i];
    }

    return resultado;
}

// Alojamiento* Sistema::filtrarAlojamientosDisponibles(
//     const string& municipio, float precioMax, float puntuacionMin, int& cantidadFiltrada
//     ) {
//     Alojamiento* resultado = new Alojamiento[numAlojamientos];
//     agregarMemoria(sizeof(Alojamiento) * numAlojamientos);
//     cantidadFiltrada = 0;

//     cout << "\n=== DEBUG FILTRADO ===" << endl;
//     cout << "Municipio buscado: " << municipio << endl;
//     cout << "Precio max: " << precioMax << endl;
//     cout << "Puntuacion min: " << puntuacionMin << endl;

//     for (int i = 0; i < numAlojamientos; ++i) {
//         incrementarIteracion();
//         cout << "\nEvaluando alojamiento " << i << ":" << endl;
//         cout << "Municipio: " << alojamientos[i].getMunicipio() << endl;
//         cout << "Precio: " << alojamientos[i].getPrecio() << endl;

//         if (alojamientos[i].getMunicipio() != municipio) {
//             cout << " - No coincide municipio" << endl;
//             continue;
//         }

//         if (precioMax >= 0 && alojamientos[i].getPrecio() > precioMax) {
//             cout << " - Excede precio maximo" << endl;
//             continue;
//         }

//         string docAnfitrion = alojamientos[i].getDocumentoAnfitrion();
//         float puntuacion = -1;

//         for (int j = 0; j < numAnfitriones; ++j) {
//             incrementarIteracion();
//             if (anfitriones[j].getDocumento() == docAnfitrion) {
//                 puntuacion = anfitriones[j].getPuntuacion();
//                 break;
//             }
//         }

//         cout << "Puntuacion anfitrion: " << puntuacion << endl;

//         if (puntuacionMin >= 0 && puntuacion < puntuacionMin) {
//             cout << " - No cumple puntuacion minima" << endl;
//             continue;
//         }

//         resultado[cantidadFiltrada++] = alojamientos[i];
//         cout << " - ACEPTADO" << endl;
//     }

//     cout << "Total encontrados: " << cantidadFiltrada << endl;
//     return resultado;
// }

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
    agregarMemoria(-sizeof(Alojamiento) * nFiltrados); // Memoria liberada

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

    agregarMemoria(sizeof(Reservacion)); // Medición de memoria

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
        incrementarIteracion();
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

        // Registrar memoria utilizada
        agregarMemoria(sizeof(Reservacion) * (numReservaciones + 1));

        // 2. Copiar reservaciones existentes
        for (int i = 0; i < numReservaciones; i++) {
            nuevasReservaciones[i] = reservaciones[i];
            incrementarIteracion();
        }

        // 3. Agregar la nueva reservacion
        nuevasReservaciones[numReservaciones] = reservacion;

        // 4. Reemplazar el array antiguo
        agregarMemoria(-sizeof(Reservacion) * numReservaciones);
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
        incrementarIteracion();
        if (reservaciones[i].getCodigo() == codigo &&
            reservaciones[i].getDocumento() == documento) {

            // Reorganizar el arreglo (no se usa delete porque son objetos, no punteros)
            for (int j = i; j < numReservaciones - 1; ++j) {
                reservaciones[j] = reservaciones[j + 1];
                incrementarIteracion();
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
        incrementarIteracion();
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
    agregarMemoria(sizeof(bool) * contadorTotal);
    int index = 0;

    for (int i = 0; i < totalReservaciones; i++) {
        incrementarIteracion();
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
    agregarMemoria(sizeof(Reservacion) * contadorTotal);
    int idxFuturas = 0;
    int idxPasadas = totalFuturas;
    index = 0;

    for (int i = 0; i < totalReservaciones; i++) {
        incrementarIteracion();
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
    agregarMemoria(-sizeof(bool) * contadorTotal);  // Memoria liberada

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
    agregarMemoria(sizeof(Reservacion) * totalEncontradas);

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
        incrementarIteracion();
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

void Sistema::mostrarEstadisticasHistorico(const Fecha& fechaCorte, int finalizadas, int activas, int enCurso) {
    cout << "\n=== RESUMEN DE ACTUALIZACION DE HISTORICO ===\n";
    cout << "Fecha de corte establecida: " << fechaCorte.toString() << endl;
    cout << "Fecha de procesamiento: " << Fecha::obtenerFechaActual() << endl;
    cout << "\n--- ESTADISTICAS DE RESERVACIONES ---\n";
    cout << "Reservaciones FINALIZADAS (movidas al historico): " << finalizadas << endl;
    cout << "Reservaciones EN CURSO (mantienen activas): " << enCurso << endl;
    cout << "Reservaciones FUTURAS (mantienen activas): " << (activas - enCurso) << endl;
    cout << "Total de reservaciones activas restantes: " << activas << endl;

    // Información adicional sobre el sistema
    cout << "\n--- CONFIGURACION DEL SISTEMA ---\n";
    Fecha fechaLimite = fechaCorte.sumarDias(365); // 12 meses aproximadamente
    cout << "Nueva fecha base para reservaciones: " << fechaCorte.toString() << endl;
    cout << "Limite de reservaciones habilitado hasta: " << fechaLimite.toString() << endl;

    // Resumen final
    cout << "\n--- ARCHIVOS ACTUALIZADOS ---\n";
    cout << "HistoricoReservaciones.txt - Nuevas " << finalizadas << " reservaciones agregadas\n";
    cout << "reservaciones.txt - Actualizado con " << activas << " reservaciones activas\n";
    cout << "fecha_corte_historico.txt - Nueva fecha base guardada\n";

    cout << "\n==========================================\n";
    cout << "Actualizacion de historico completada exitosamente.\n";
    cout << "==========================================\n";
}

void Sistema::procesarActualizacionHistorico(const Fecha& fechaCorte) {
    cout << "\n=== ACTUALIZACION DE HISTORICO ===\n";

    Fecha fechaHoy(Fecha::obtenerFechaActual());

    int reservacionesFinalizadas = 0;
    int reservacionesActivas = 0;
    int reservacionesEnCurso = 0;

    for (int i = 0; i < numReservaciones; i++) {
        incrementarIteracion();
        Fecha fechaInicio(reservaciones[i].getFechaEntrada());
        Fecha fechaFin = fechaInicio.calcularFechaFinal(reservaciones[i].getDuracion());

        if (fechaFin < fechaHoy) {
            reservacionesFinalizadas++;
        } else {
            reservacionesActivas++;
            if (fechaInicio <= fechaHoy && fechaFin >= fechaHoy) {
                reservacionesEnCurso++;
            }
        }
    }

    cout << "- Finalizadas: " << reservacionesFinalizadas << endl;
    cout << "- En curso: " << reservacionesEnCurso << endl;
    cout << "- Futuras: " << (reservacionesActivas - reservacionesEnCurso) << endl;

    if (reservacionesFinalizadas == 0) {
        gestionArchivos->guardarUltimaFechaCorteHistorico(fechaCorte.toString());
        cout << "Fecha base actualizada: " << fechaCorte.sumarDias(365).toString() << endl;
        return;
    }

    cout << "Mover " << reservacionesFinalizadas << " al historico? (s/n): ";
    char confirmar;
    cin >> confirmar;
    if (confirmar != 's' && confirmar != 'S') return;

    Reservacion* reservacionesParaHistorico = new Reservacion[reservacionesFinalizadas];
    agregarMemoria(sizeof(Reservacion) * reservacionesFinalizadas);

    Reservacion* reservacionesParaMantener = new Reservacion[reservacionesActivas];
    agregarMemoria(sizeof(Reservacion) * reservacionesActivas);

    int iH = 0, iM = 0;
    for (int i = 0; i < numReservaciones; i++) {
        incrementarIteracion();
        Fecha inicio(reservaciones[i].getFechaEntrada());
        Fecha fin = inicio.calcularFechaFinal(reservaciones[i].getDuracion());
        if (fin < fechaHoy) {
            reservacionesParaHistorico[iH++] = reservaciones[i];
        } else {
            reservacionesParaMantener[iM++] = reservaciones[i];
        }
    }

    if (gestionArchivos->guardarReservacionesHistorico(reservacionesParaHistorico, reservacionesFinalizadas, fechaCorte.toString())) {

        agregarMemoria(-sizeof(Reservacion) * numReservaciones);
        delete[] reservaciones;

        reservaciones = reservacionesParaMantener;
        numReservaciones = reservacionesActivas;

        gestionArchivos->actualizarArchivoReservaciones(reservaciones, numReservaciones);
        gestionArchivos->guardarUltimaFechaCorteHistorico(fechaCorte.toString());
        cout << "Actualizacion completada. Activas: " << reservacionesActivas << endl;
        mostrarEstadisticasHistorico(fechaCorte, reservacionesFinalizadas, reservacionesActivas, reservacionesEnCurso);
    } else {
        cout << "Error al guardar en historico.\n";

        agregarMemoria(-sizeof(Reservacion) * reservacionesActivas);
        delete[] reservacionesParaMantener;
    }

    agregarMemoria(-sizeof(Reservacion) * reservacionesFinalizadas);
    delete[] reservacionesParaHistorico;
}
