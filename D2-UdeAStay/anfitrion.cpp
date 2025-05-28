#include "anfitrion.h"
#include "alojamiento.h"
#include "sistema.h"
#include "reservacion.h"

#include <iostream>
#include <iomanip>

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

// Destructor
Anfitrion::~Anfitrion() {
    // No es necesario liberar recursos manualmente porque no hay uso de memoria dinámica propia
}

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
    fechaInicio = Fecha(fechaStr);

    cout << "Fecha de fin (DD/MM/AAAA): ";
    cin >> fechaStr;
    fechaFin = Fecha(fechaStr);

    // Validar rango
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

    if (reservaciones == nullptr) {
        cout << "No se encontraron reservaciones activas en el rango de fechas especificado.\n\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    cout << "\n=== RESERVACIONES ENCONTRADAS (" << totalEncontradas << ") ===\n";
    cout << "Rango: " << fechaInicio.toString() << " - " << fechaFin.toString() << "\n\n";

    for (int i = 0; i < totalEncontradas; i++) {
        reservaciones[i].mostrarDetalle(i + 1, true, sistema);
    }

    cout << "\nTotal de reservaciones activas: " << totalEncontradas << "\n\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();

    delete[] reservaciones;
}

void Anfitrion::cancelarReservacion(Sistema* sistema) {
    cout << "\n=== CANCELAR RESERVACION ===\n";

    // Primero mostrar las reservaciones activas del anfitrión
    int* indicesReservaciones = new int[sistema->getNumReservaciones()];
    int numReservacionesCancelables = 0;

    cout << "Reservaciones cancelables (futuras) asociadas a sus alojamientos:\n";
    cout << "--------------------------------------------------------\n";

    bool tieneReservaciones = false;
    int contador = 1;

    // Obtener todas las reservaciones y filtrar por alojamientos del anfitrión
    const Reservacion* reservaciones = sistema->getReservaciones();
    const Alojamiento* alojamientos = sistema->getAlojamientos();

    for (int i = 0; i < sistema->getNumReservaciones(); ++i) {
        const Reservacion& reservacion = reservaciones[i];

        // Verificar si el alojamiento pertenece a este anfitrión
        bool esDelAnfitrion = false;
        for (int j = 0; j < sistema->getNumAlojamientos(); ++j) {
            const Alojamiento& alojamiento = alojamientos[j];
            if (alojamiento.getCodigo() == reservacion.getCodigoAlojamiento() &&
                alojamiento.getDocumentoAnfitrion() == this->getDocumento()) {
                esDelAnfitrion = true;
                break;
            }
        }

        // Verificar que la reservación no haya pasado
        Fecha fechaEntrada(reservacion.getFechaEntrada());
        bool noHaPasado = !fechaEntrada.yaPaso();

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

    if (!tieneReservaciones) {
        cout << "No tiene reservaciones cancelables (futuras) en sus alojamientos.\n";
        cout << "Las reservaciones que ya comenzaron no se pueden cancelar.\n";
        delete[] indicesReservaciones; // Liberar memoria antes de salir
        return;
    }

    int seleccion;
    while (true) {
        cout << "\nSeleccione el numero de la reservacion a cancelar (1-" << contador - 1 << ") o 0 para cancelar: ";
        cin >> seleccion;

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
            return;
        }

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
    cout << "Estado: Reservacion: Pendiente\n";

    cout << "\nEsta seguro de que desea cancelar esta reservacion? (S/N): ";
    char confirmacion;
    cin >> confirmacion;

    if (tolower(confirmacion) != 's') {
        cout << "Cancelacion abortada.\n";
        delete[] indicesReservaciones; // Liberar memoria antes de salir
        return;
    }

    // Proceder con la cancelación
    bool exito = sistema->eliminarReservacionPorCodigo(
        reservacionSeleccionada.getCodigo(),
        reservacionSeleccionada.getDocumento()
        );

    if (exito) {
        cout << "\nReservacion cancelada exitosamente.\n";
        cout << "El huesped sera notificado de la cancelacion.\n";
    } else {
        cout << "\nError: No se pudo cancelar la reservacion. ";
        cout << "Verifique que la reservacion aun existe.\n";
    }

    // Liberar memoria antes de salir
    delete[] indicesReservaciones;
}

void Anfitrion::actualizarHistorico(Sistema* sistema) {
    cout << "\n=== ACTUALIZAR HISTORICO DE RESERVACIONES ===\n";

    // Solicitar fecha de corte al anfitrion
    string fechaCorteStr;
    Fecha fechaCorte;
    bool fechaValida = false;

    do {
        cout << "Ingrese la fecha de corte (DD/MM/AAAA): ";
        cin >> fechaCorteStr;

        try {
            fechaCorte = Fecha(fechaCorteStr);

            if (!fechaCorte.esValida()) {
                cout << "Error: Fecha no valida. Intente nuevamente.\n";
                continue;
            }

            // Validar que no sea menor a la fecha registrada
            string ultimaFechaStr = sistema->obtenerUltimaFechaCorteHistorico();

            if (!ultimaFechaStr.empty()) {
                Fecha ultimaFechaCorte(ultimaFechaStr);
                if (ultimaFechaCorte.esValida() && fechaCorte <= ultimaFechaCorte) {
                    cout << "Error: La fecha de corte (" << fechaCorte.toString()
                    << ") debe ser posterior a la ultima fecha registrada ("
                    << ultimaFechaCorte.toString() << ").\n";
                    cout << "Por favor ingrese una fecha mas reciente.\n";
                    continue;
                }
                cout << "Ultima fecha de corte registrada: " << ultimaFechaCorte.toString() << "\n";
            } else {
                cout << "Esta es la primera actualizacion de historico del sistema.\n";
            }

            cout << "\n=== INFORMACION IMPORTANTE ===\n";
            cout << "LOGICA DEL SISTEMA: Solo se mueven al historico las reservaciones YA FINALIZADAS.\n";
            cout << "Una reservacion esta finalizada cuando su fecha de salida ya paso.\n\n";

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

            fechaValida = true;

        } catch (...) {
            cout << "Error: Formato de fecha incorrecto. Use DD/MM/AAAA\n";
        }

    } while (!fechaValida);

    cout << "\nFecha de corte establecida: " << fechaCorte.toString() << endl;
    cout << "RECORDATORIO: Solo se moveran reservaciones YA FINALIZADAS al historico.\n";
    cout << "Confirma la operacion? (s/n): ";

    char confirmacion;
    cin >> confirmacion;

    if (confirmacion != 's' && confirmacion != 'S') {
        cout << "Operacion cancelada.\n";
        return;
    }

    // Procesar actualización de histórico
    sistema->procesarActualizacionHistorico(fechaCorte);
}
