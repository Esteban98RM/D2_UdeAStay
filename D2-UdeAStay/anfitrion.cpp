#include "anfitrion.h"
#include "alojamiento.h"
#include "sistema.h"
#include "reservacion.h"
#include "medicionrecursos.h"

#include <iostream>
#include <iomanip>

using namespace std;

/**
 * @brief Constructor por defecto de la clase Anfitrion.
 *
 * Inicializa todos los atributos con valores predeterminados:
 * cadenas vacías, antigüedad en 0 y puntuación en 0.0.
 * También registra la memoria usada.
 */
Anfitrion::Anfitrion() : codigo(""), documento(""), clave(""), antiguedad(0), puntuacion(0.0f) {
    agregarMemoria(sizeof(string) * 3 + sizeof(int) + sizeof(float));
}

/**
 * @brief Constructor con parámetros de la clase Anfitrion.
 *
 * Inicializa los atributos con los valores dados y realiza validaciones:
 * - Ningún campo de texto debe estar vacío.
 * - La antigüedad no puede ser negativa.
 * - La puntuación debe estar en el rango [0.0, 5.0].
 *
 * @param cod Código identificador del anfitrión.
 * @param doc Documento de identidad del anfitrión.
 * @param clv Clave o contraseña del anfitrión.
 * @param ant Antigüedad en años del anfitrión.
 * @param punt Puntuación promedio del anfitrión.
 *
 * @throws std::invalid_argument si se detectan datos inválidos.
 */
Anfitrion::Anfitrion(const string& cod, const string& doc, const string& clv, int ant, float punt) :
    codigo(cod), documento(doc), clave(clv), antiguedad(ant), puntuacion(punt) {

    agregarMemoria(sizeof(string) * 3 + sizeof(int) + sizeof(float)); // Memoria para atributos
    incrementarIteracion(); // Validación de campos vacíos

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

/**
 * @brief Destructor de la clase Anfitrion.
 *
 * No realiza liberación explícita de memoria porque no se usa asignación dinámica.
 */
Anfitrion::~Anfitrion() {
    // No es necesario liberar recursos manualmente porque no hay uso de memoria dinámica propia
}

/**
 * @brief Muestra en consola las reservaciones activas de los alojamientos de un anfitrión.
 *
 * Solicita al usuario un rango de fechas, consulta el sistema por las reservaciones activas
 * del anfitrión en ese rango y las muestra en pantalla. También gestiona recursos temporales
 * y actualiza estadísticas de memoria e iteraciones.
 *
 * @param sistema Puntero al objeto Sistema desde el cual se obtienen las reservaciones.
 *
 * El proceso incluye:
 * - Solicitud de fechas de inicio y fin al usuario.
 * - Validación del rango de fechas.
 * - Consulta de reservaciones activas pertenecientes al anfitrión.
 * - Muestra en consola de los detalles de cada reservación encontrada.
 * - Manejo de memoria dinámica asociada a las reservaciones obtenidas.
 *
 * Si no se encuentran reservaciones activas en el rango dado, se notifica al usuario.
 */
void Anfitrion::mostrarReservacionesActivas(Sistema* sistema) {
    cout << "\n=== RESERVACIONES ACTIVAS DE MIS ALOJAMIENTOS ===\n";
    cout << "Anfitrion: " << this->getCodigo() << "\n";
    cout << "Documento: " << this->getDocumento() << "\n\n";

    // Solicitar rango de fechas
    Fecha fechaInicio, fechaFin;
    cout << "Ingrese el rango de fechas para consultar:\n";

    cout << "Fecha de inicio (DD/MM/AAAA): ";
    string fechaStr;
    cin >> fechaStr;

    incrementarIteracion();
    fechaInicio = Fecha(fechaStr);

    cout << "Fecha de fin (DD/MM/AAAA): ";
    cin >> fechaStr;

    incrementarIteracion();
    fechaFin = Fecha(fechaStr);

    // Validar rango
    incrementarIteracion();
    if (fechaFin < fechaInicio) {
        cout << "Error: La fecha de fin debe ser posterior a la fecha de inicio.\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    int totalEncontradas;
    Reservacion* reservaciones = sistema->obtenerReservaciones(
        this->getDocumento(), fechaInicio, fechaFin, totalEncontradas);

    incrementarIteracion();
    if (reservaciones == nullptr) {
        cout << "No se encontraron reservaciones activas en el rango de fechas especificado.\n\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    agregarMemoria(sizeof(Reservacion) * totalEncontradas);

    cout << "\n=== RESERVACIONES ENCONTRADAS (" << totalEncontradas << ") ===\n";
    cout << "Rango: " << fechaInicio.toString() << " - " << fechaFin.toString() << "\n\n";

    for (int i = 0; i < totalEncontradas; i++) {
        incrementarIteracion();
        reservaciones[i].mostrarDetalle(i + 1, true, sistema);
    }

    cout << "\nTotal de reservaciones activas: " << totalEncontradas << "\n\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();

    delete[] reservaciones;
    agregarMemoria(-sizeof(Reservacion) * totalEncontradas);
}

/**
 * @brief Permite a un anfitrión cancelar una reservación futura asociada a sus alojamientos.
 *
 * Esta función muestra una lista de reservaciones futuras (aún no iniciadas) correspondientes
 * a los alojamientos administrados por el anfitrión. Luego permite seleccionar una reservación
 * para cancelarla, previa confirmación del usuario. La operación actualiza estadísticas internas
 * de memoria e iteraciones.
 *
 * @param sistema Puntero al sistema central que contiene la información de alojamientos y reservaciones.
 *
 * El flujo general es el siguiente:
 * - Se filtran reservaciones futuras asociadas al anfitrión.
 * - Se muestran en consola con sus detalles relevantes.
 * - El anfitrión puede seleccionar una para cancelar o abortar la operación.
 * - Se solicita confirmación antes de proceder.
 * - En caso de éxito, se informa la cancelación al usuario.
 *
 * Se maneja la memoria dinámica usada para registrar los índices de reservaciones válidas
 * y se asegura su liberación sin importar el resultado del proceso.
 *
 * Validaciones incluidas:
 * - Que existan reservaciones futuras asociadas al anfitrión.
 * - Que el rango de selección del usuario sea válido.
 * - Confirmación final del anfitrión antes de proceder con la cancelación.
 */
void Anfitrion::cancelarReservacion(Sistema* sistema) {
    cout << "\n=== CANCELAR RESERVACION ===\n";

    // Primero mostrar las reservaciones activas del anfitrión
    int* indicesReservaciones = new int[sistema->getNumReservaciones()];
    agregarMemoria(sizeof(int) * sistema->getNumReservaciones());

    int numReservacionesCancelables = 0;

    cout << "Reservaciones cancelables (futuras) asociadas a sus alojamientos:\n";
    cout << "--------------------------------------------------------\n";

    bool tieneReservaciones = false;
    int contador = 1;

    // Obtener todas las reservaciones y filtrar por alojamientos del anfitrión
    const Reservacion* reservaciones = sistema->getReservaciones();
    const Alojamiento* alojamientos = sistema->getAlojamientos();

    for (int i = 0; i < sistema->getNumReservaciones(); ++i) {
        incrementarIteracion();
        const Reservacion& reservacion = reservaciones[i];

        // Verificar si el alojamiento pertenece a este anfitrión
        bool esDelAnfitrion = false;
        for (int j = 0; j < sistema->getNumAlojamientos(); ++j) {
            incrementarIteracion();
            const Alojamiento& alojamiento = alojamientos[j];
            incrementarIteracion();
            if (alojamiento.getCodigo() == reservacion.getCodigoAlojamiento() &&
                alojamiento.getDocumentoAnfitrion() == this->getDocumento()) {
                esDelAnfitrion = true;
                break;
            }
        }

        // Verificar que la reservación no haya pasado
        Fecha fechaEntrada(reservacion.getFechaEntrada());
        incrementarIteracion();
        bool noHaPasado = !fechaEntrada.yaPaso();
        incrementarIteracion();

        if (esDelAnfitrion && noHaPasado) {
            tieneReservaciones = true;
            indicesReservaciones[numReservacionesCancelables] = i;
            numReservacionesCancelables++;

            cout << contador << ". Codigo: " << reservacion.getCodigo() << "\n";
            cout << "   Fecha entrada: " << reservacion.getFechaEntrada() << "\n";

            // Calcular fecha de salida
            Fecha fechaSalida = fechaEntrada.sumarDias(reservacion.getDuracion());
            cout << "   Fecha salida: " << fechaSalida.toString() << "\n";

            cout << "   Alojamiento: " << reservacion.getCodigoAlojamiento() << "\n";
            cout << "   Documento huesped: " << reservacion.getDocumento() << "\n";
            cout << "   Monto: $" << fixed << setprecision(0) << reservacion.getMonto() << "\n";
            cout << "   ----------------------------------------\n";

            contador++;
        }
    }
    incrementarIteracion();

    if (!tieneReservaciones) {
        cout << "No tiene reservaciones cancelables (futuras) en sus alojamientos.\n";
        cout << "Las reservaciones que ya comenzaron no se pueden cancelar.\n";
        delete[] indicesReservaciones; // Liberar memoria antes de salir
        agregarMemoria(-sizeof(int) * sistema->getNumReservaciones());
        return;
    }

    int seleccion;
    while (true) {
        cout << "\nSeleccione el numero de la reservacion a cancelar (1-" << contador - 1 << ") o 0 para cancelar: ";
        cin >> seleccion;
        incrementarIteracion();

        if (!cin) {
            cin.clear();
            string basura;
            cin >> basura; // consume el texto inválido
            cout << "Entrada invalida. Debe ingresar un numero.\n";
            continue;
        }

        if (seleccion == 0) {
            cout << "Cancelando operacion.\n";
            delete[] indicesReservaciones;
            agregarMemoria(-sizeof(int) * sistema->getNumReservaciones());
            return;
        }

        incrementarIteracion();
        if (seleccion >= 1 && seleccion <= contador - 1) {
            break; // selección válida
        }

        cout << "Numero fuera del rango valido.\n";
    }

    // Obtener la reservación seleccionada
    int indiceReservacion = indicesReservaciones[seleccion - 1];
    const Reservacion& reservacionSeleccionada = reservaciones[indiceReservacion];

    // Mostrar confirmacion
    cout << "\nReservacion seleccionada:\n";
    cout << "Codigo: " << reservacionSeleccionada.getCodigo() << "\n";
    cout << "Huesped: " << reservacionSeleccionada.getDocumento() << "\n";
    cout << "Fecha entrada: " << reservacionSeleccionada.getFechaEntrada() << "\n";
    cout << "Alojamiento: " << reservacionSeleccionada.getCodigoAlojamiento() << "\n";
    cout << "Monto: $" << fixed << setprecision(0) << reservacionSeleccionada.getMonto() << "\n";
    cout << "Estado: Pendiente\n";

    cout << "\nEsta seguro de que desea cancelar esta reservacion? (s/n): ";
    char confirmacion;
    cin >> confirmacion;
    incrementarIteracion();

    if (tolower(confirmacion) != 's') {
        cout << "Cancelacion abortada.\n";
        delete[] indicesReservaciones; // Liberar memoria antes de salir
        agregarMemoria(-sizeof(int) * sistema->getNumReservaciones());
        return;
    }

    // Proceder con la cancelación
    bool exito = sistema->eliminarReservacionPorCodigo(
        reservacionSeleccionada.getCodigo(),
        reservacionSeleccionada.getDocumento()
        );
    incrementarIteracion();

    if (exito) {
        cout << "\nReservacion cancelada exitosamente.\n";
        cout << "El huesped sera notificado de la cancelacion.\n";
    } else {
        cout << "\nError: No se pudo cancelar la reservacion. ";
        cout << "Verifique que la reservacion aun existe.\n";
    }

    // Liberar memoria antes de salir
    delete[] indicesReservaciones;
    agregarMemoria(-sizeof(int) * sistema->getNumReservaciones());
}

/**
 * @brief Solicita una fecha de corte al anfitrión y actualiza el histórico de reservaciones.
 *
 * Esta función permite definir una nueva fecha de corte en el sistema para trasladar al histórico
 * aquellas reservaciones finalizadas (cuyo periodo ya ha pasado completamente). Se asegura que la
 * fecha ingresada sea válida y posterior a la última fecha de corte registrada, si existe.
 *
 * @param sistema Puntero al sistema central que administra las reservaciones y el histórico.
 *
 * El procedimiento incluye:
 * - Solicitar y validar la fecha de corte ingresada por el anfitrión.
 * - Verificar que la fecha no sea anterior o igual a la última fecha de corte ya registrada.
 * - Mostrar información relevante sobre cómo se clasifican las reservaciones para ser trasladadas.
 * - Confirmar la operación con el anfitrión antes de continuar.
 * - Llamar a `Sistema::procesarActualizacionHistorico` para ejecutar la actualización.
 *
 * Validaciones importantes:
 * - Formato y validez de la fecha.
 * - Fecha mayor a la última registrada.
 * - Confirmación del usuario antes de proceder.
 *
 * Se actualiza el contador de iteraciones durante todas las operaciones relevantes
 * y se informa claramente al usuario de las implicaciones de la operación.
 */
void Anfitrion::actualizarHistorico(Sistema* sistema) {
    cout << "\n=== ACTUALIZAR HISTORICO DE RESERVACIONES ===\n";

    // Solicitar fecha de corte al anfitrion
    string fechaCorteStr;
    Fecha fechaCorte;
    bool fechaValida = false;

    do {
        cout << "Ingrese la fecha de corte (DD/MM/AAAA): ";
        cin >> fechaCorteStr;
        incrementarIteracion();

        try {
            fechaCorte = Fecha(fechaCorteStr);

            incrementarIteracion();
            if (!fechaCorte.esValida()) {
                cout << "Error: Fecha no valida. Intente nuevamente.\n";
                incrementarIteracion();
                continue;
            }
            incrementarIteracion();

            // Validar que no sea menor a la fecha registrada
            string ultimaFechaStr = sistema->obtenerUltimaFechaCorteHistorico();
            incrementarIteracion();

            if (!ultimaFechaStr.empty()) {
                Fecha ultimaFechaCorte(ultimaFechaStr);
                incrementarIteracion();
                if (ultimaFechaCorte.esValida() && fechaCorte <= ultimaFechaCorte) {
                    cout << "Error: La fecha de corte (" << fechaCorte.toString()
                    << ") debe ser posterior a la ultima fecha registrada ("
                    << ultimaFechaCorte.toString() << ").\n";
                    cout << "Por favor ingrese una fecha mas reciente.\n";
                    incrementarIteracion();
                    continue;
                }
                cout << "Ultima fecha de corte registrada: " << ultimaFechaCorte.toString() << "\n";
                incrementarIteracion();
            } else {
                cout << "Esta es la primera actualizacion de historico del sistema.\n";
                incrementarIteracion();
            }

            cout << "\n=== INFORMACION IMPORTANTE ===\n";
            cout << "Solo se mueven al historico las reservaciones YA FINALIZADAS.\n";
            cout << "Una reservacion esta finalizada cuando su fecha de salida ya paso.\n\n";
            incrementarIteracion();

            if (fechaCorte.yaPaso()) {
                cout << "Fecha de corte: " << fechaCorte.toString() << " (fecha pasada)\n";
                cout << "- Se moveran al historico: Reservaciones completamente finalizadas\n";
                cout << "- Se mantendran activas: Reservaciones en curso y futuras\n";
            } else {
                cout << "Fecha de corte: " << fechaCorte.toString() << " (fecha futura)\n";
                cout << "- Se moveran al historico: Solo reservaciones ya finalizadas al dia de hoy\n";
                cout << "- La fecha de corte establece una nueva base para el sistema\n";
                cout << "- NO se moveran reservaciones futuras o en curso\n";
            }
            cout << "============================\n";
            incrementarIteracion();

            fechaValida = true;

        } catch (...) {
            cout << "Error: Formato de fecha incorrecto. Use DD/MM/AAAA\n";
            incrementarIteracion();
        }

    } while (!fechaValida);

    cout << "\nFecha de corte establecida: " << fechaCorte.toString() << endl;
    cout << "RECORDATORIO: Solo se moveran reservaciones YA FINALIZADAS al historico.\n";
    cout << "Confirma la operacion? (s/n): ";

    char confirmacion;
    cin >> confirmacion;
    incrementarIteracion();

    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Operacion cancelada.\n";
        incrementarIteracion();
        return;
    }

    // Procesar actualización de histórico
    sistema->procesarActualizacionHistorico(fechaCorte);
    incrementarIteracion();
}
