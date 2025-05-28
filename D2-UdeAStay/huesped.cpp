#include "huesped.h"
#include "sistema.h"
#include "alojamiento.h"
#include "anfitrion.h"
#include "fecha.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <cstdlib>

// Constructor por defecto
Huesped::Huesped() : nombre(""), documento(""), clave(""), antiguedad(0), puntuacion(0.0f),
reservaciones(nullptr), numReservaciones(0), capacidadReservaciones(0){}

// Constructor con parametros
Huesped::Huesped(const string& nom, const string& doc, const string& clv, int ant, float punt) :
    nombre(nom), documento(doc), clave(clv), antiguedad(ant), puntuacion(punt),
    reservaciones(nullptr), numReservaciones(0), capacidadReservaciones(0)
{

    // Validacion basica de datos
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

Huesped::Huesped(const Huesped& otro) {
    nombre = otro.nombre;
    documento = otro.documento;
    clave = otro.clave;
    antiguedad = otro.antiguedad;
    puntuacion = otro.puntuacion;
    numReservaciones = otro.numReservaciones;
    capacidadReservaciones = otro.capacidadReservaciones;

    reservaciones = new Reservacion[capacidadReservaciones];
    for (int i = 0; i < numReservaciones; i++) {
        reservaciones[i] = otro.reservaciones[i];
    }
}

Huesped::~Huesped() {
    delete[] reservaciones;
    reservaciones = nullptr;
}

Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservaciones;

        nombre = otro.nombre;
        documento = otro.documento;
        clave = otro.clave;
        antiguedad = otro.antiguedad;
        puntuacion = otro.puntuacion;
        numReservaciones = otro.numReservaciones;
        capacidadReservaciones = otro.capacidadReservaciones;

        reservaciones = new Reservacion[capacidadReservaciones];
        for (int i = 0; i < numReservaciones; i++) {
            reservaciones[i] = otro.reservaciones[i];
        }
    }
    return *this;
}

void Huesped::expandirCapacidadReservaciones() {
    int nuevaCapacidad = (capacidadReservaciones == 0) ? 2 : capacidadReservaciones * 2;
    Reservacion* nuevoArray = new Reservacion[nuevaCapacidad];

    for (int i = 0; i < numReservaciones; i++) {
        nuevoArray[i] = reservaciones[i];
    }

    delete[] reservaciones;
    reservaciones = nuevoArray;
    capacidadReservaciones = nuevaCapacidad;
}

Fecha Huesped::solicitarFechaEntrada() {
    Fecha fechaEntrada;
    string fechaStr;

    while (true) {
        cout << "Ingrese fecha de entrada (DD/MM/AAAA): ";
        cin >> fechaStr;

        fechaEntrada = Fecha(fechaStr);
        if (!fechaEntrada.esValida()) {
            cout << "Fecha invalida. Formato correcto: DD/MM/AAAA\n";
            continue;
        }
        if (fechaEntrada.yaPaso()) {
            cout << "La fecha ya ha pasado. Ingrese una fecha futura.\n";
            continue;
        }
        break;
    }
    return fechaEntrada;
}

string Huesped::solicitarMunicipio() {
    string municipio;
    cout << "Ingrese municipio de destino: ";
    cin.ignore();
    getline(cin, municipio);

    if (municipio.empty()) {
        cout << "El municipio no puede estar vacio.\n";
        return solicitarMunicipio(); // Recursion para reintentar
    }
    return municipio;
}

int Huesped::solicitarCantidadNoches() {
    int noches;
    while (true) {
        cout << "Ingrese cantidad de noches: ";
        string entrada;
        cin >> entrada;
        try {
            noches = stoi(entrada);
            if (noches <= 0) throw invalid_argument("Debe ser mayor que 0.");
            break;
        } catch (...) {
            cout << "Cantidad invalida. Ingrese un numero entero positivo.\n";
        }
    }
    return noches;
}

float Huesped::solicitarPrecioMaximo() {
    char opc;
    cout << "Desea ingresar un precio maximo? (s/n): ";
    cin >> opc;

    if (opc != 's' && opc != 'S') return -1; // Sin limite

    float precio;
    while (true) {
        cout << "Ingrese precio maximo: ";
        string entrada;
        cin >> entrada;
        try {
            precio = stof(entrada);
            if (precio < 0) throw invalid_argument("Negativo");
            break;
        } catch (...) {
            cout << "Precio invalido. Ingrese un numero valido mayor o igual a 0.\n";
        }
    }
    return precio;
}

float Huesped::solicitarPuntuacionMinima() {
    char opc;
    cout << "Desea ingresar una puntuacion minima? (s/n): ";
    cin >> opc;

    if (opc != 's' && opc != 'S') return -1; // Sin limite

    float puntuacion;
    while (true) {
        cout << "Ingrese puntuacion minima (0.0 - 5.0): ";
        string entrada;
        cin >> entrada;
        try {
            puntuacion = stof(entrada);
            if (puntuacion < 0.0f || puntuacion > 5.0f)
                throw out_of_range("Fuera de rango");
            break;
        } catch (...) {
            cout << "Puntuacion invalida. Debe estar entre 0.0 y 5.0.\n";
        }
    }
    return puntuacion;
}

char Huesped::solicitarMetodoPago() {
    char metodo;
    while (true) {
        cout << "\nMetodos de pago disponibles:\n";
        cout << "[T] Tarjeta de credito\n";
        cout << "[P] PSE\n";
        cout << "Seleccione metodo de pago: ";
        cin >> metodo;

        metodo = toupper(metodo);
        if (metodo == 'T' || metodo == 'P') {
            break;
        }
        cout << "Opcion invalida. Intente nuevamente.\n";
    }
    return metodo;
}

string Huesped::solicitarAnotaciones() {
    string anotaciones;
    char opc;
    cout << "Desea agregar anotaciones especiales? (s/n): ";
    cin >> opc;

    if (opc == 's' || opc == 'S') {
        cout << "Ingrese sus anotaciones: ";
        cin.ignore();
        getline(cin, anotaciones);
    }
    return anotaciones;
}

int Huesped::seleccionarAlojamiento(Alojamiento* disponibles, int cantidad) {
    // Mostrar opciones
    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    for (int i = 0; i < cantidad; ++i) {
        cout << "[" << (i + 1) << "] ";
        disponibles[i].mostrar();
        cout << "\n";
    }

    // Seleccion
    int seleccion;
    while (true) {
        cout << "Seleccione una opcion (1-" << cantidad << ") o 0 para cancelar: ";
        cin >> seleccion;

        if (seleccion == 0) return -1; // Cancelar
        if (seleccion >= 1 && seleccion <= cantidad) {
            return seleccion - 1; // indice basado en 0
        }
        cout << "Opcion invalida.\n";
    }
}

// Implementacion del metodo principal
void Huesped::ReservarAlojamiento(Sistema* sistema) {
    while (true) {
        // 1. Recopilar datos del usuario
        Fecha fechaEntrada = solicitarFechaEntrada();
        string municipio = solicitarMunicipio();
        int noches = solicitarCantidadNoches();
        float precioMax = solicitarPrecioMaximo();
        float puntuacionMin = solicitarPuntuacionMinima();

        // 2. Buscar alojamientos disponibles
        int nDisponibles = 0;
        Alojamiento* disponibles = sistema->buscarAlojamientosDisponibles(
            municipio, fechaEntrada, noches, precioMax, puntuacionMin, nDisponibles
            );

        if (nDisponibles == 0) {
            char opcion;
            cout << "\nNo hay alojamientos disponibles con esos criterios.\n";
            cout << "Desea intentar con otros criterios? (s/n): ";
            cin >> opcion;

            if (tolower(opcion) == 'n') {
                return; // Salir al menu principal
            } else {
                continue; // Repetir el proceso
            }
        }

        // 3. Permitir seleccion
        int seleccion = seleccionarAlojamiento(disponibles, nDisponibles);
        if (seleccion == -1) {
            delete[] disponibles;
            return; // Usuario cancelo
        }

        // 4. Crear reservacion
        Reservacion* nuevaReservacion = sistema->crearReservacion(
            disponibles[seleccion], fechaEntrada, noches, this
            );

        // 5. Mostrar confirmacion
        mostrarConfirmacionReservacion(*nuevaReservacion, disponibles[seleccion]);

        delete[] disponibles;
        break; // exito -> salir del ciclo
    }
}

//this->getDocumento()

void Huesped::mostrarConfirmacionReservacion(const Reservacion& reservacion,
                                             const Alojamiento& alojamiento) {
    // Crear fechas para formateo completo
    Fecha fechaEntrada(reservacion.getFechaEntrada());
    Fecha fechaSalida = fechaEntrada.calcularFechaFinal(reservacion.getDuracion());

    cout << "\n" << string(60, '=') << "\n";
    cout << "            COMPROBANTE DE CONFIRMACION\n";
    cout << string(60, '=') << "\n";
    cout << "Codigo de reserva: " << reservacion.getCodigo() << "\n";
    cout << "Nombre del usuario: " << this->getNombre() << "\n";
    cout << "Documento del usuario: " << this->getDocumento() << "\n";
    cout << "Codigo del alojamiento: " << alojamiento.getCodigo() << "\n";
    cout << "Alojamiento: " << alojamiento.getNombre() << "\n";
    cout << "Municipio: " << alojamiento.getMunicipio() << "\n";
    cout << "\nFECHAS DE ESTADIA:\n";
    cout << "Fecha de inicio: " << fechaEntrada.toStringCompleto() << "\n";
    cout << "Fecha de finalizacion: " << fechaSalida.toStringCompleto() << "\n";
    cout << "Duracion: " << reservacion.getDuracion() << " noche(s)\n";
    cout << "\nDETALLES FINANCIEROS:\n";
    cout << "Precio por noche: $" << fixed << setprecision(0)
         << alojamiento.getPrecio() << "\n";
    cout << "Monto total: $" << fixed << setprecision(0)
         << reservacion.getMonto() << "\n";
    cout << "Metodo de pago: " << reservacion.getMetodoPago() << "\n";
    cout << "Fecha de pago: " << reservacion.getFechaPago() << "\n";
    if (!reservacion.getAnotacion().empty()) {
        cout << "\nAnotaciones especiales: " << reservacion.getAnotacion() << "\n";
    }
    cout << string(60, '=') << "\n";
    cout << "Reservacion confirmada exitosamente!\n";
    cout << "Que disfrute su estadia!\n";
}

void Huesped::mostrarReservaciones(Sistema* sistema) {
    cout << "\n=== LAS RESERVACIONES DEL HUESPED ===\n";
    cout << "Nombre: " << this->getNombre() << "\n";
    cout << "Documento: " << this->getDocumento() << "\n\n";

    int totalFuturas, totalPasadas;
    Reservacion* reservaciones = sistema->obtenerReservaciones(
        this->getDocumento(), totalFuturas, totalPasadas);

    if (reservaciones == nullptr) {
        cout << "No tiene reservaciones registradas.\n\n";
        cout << "Presione Enter para continuar...";
        cin.ignore();
        cin.get();
        return;
    }

    // Mostrar reservaciones futuras
    if (totalFuturas > 0) {
        cout << "=== RESERVACIONES FUTURAS (" << totalFuturas << ") ===\n";
        for (int i = 0; i < totalFuturas; i++) {
            reservaciones[i].mostrarDetalle(i + 1, false, sistema);
        }
    } else {
        cout << "No tiene reservaciones futuras.\n";
    }

    cout << "\n";

    // Mostrar reservaciones pasadas
    if (totalPasadas > 0) {
        cout << "=== RESERVACIONES PASADAS (" << totalPasadas << ") ===\n";
        for (int i = totalFuturas; i < totalFuturas + totalPasadas; i++) {
            reservaciones[i].mostrarDetalle(i - totalFuturas + 1, false, sistema);
        }
    } else {
        cout << "No tiene reservaciones pasadas.\n";
    }
    delete[] reservaciones;

    cout << "\nTotal de reservaciones: " << (totalFuturas + totalPasadas) << "\n\n";
    cout << "Presione Enter para continuar...";
    cin.ignore();
    cin.get();

}

void Huesped::anularReservacion(Sistema* sistema) {
    string numeroReserva;
    string codigoFinal;
    string codigoCompleto;
    bool formatoValido = false;

    while (!formatoValido) {
        try {
            cout << "Ingrese el numero de la reservacion: ";
            cin >> numeroReserva;

            // Validar que sea numerico
            if (numeroReserva.empty() || !all_of(numeroReserva.begin(), numeroReserva.end(), ::isdigit)) {
                throw invalid_argument("El numero de la reservacion debe contener solo digitos.");
            }

            cout << "Ingrese el codigo final : ";
            cin >> codigoFinal;

            // Validar longitud
            if (codigoFinal.length() != 5) {
                throw invalid_argument("El codigo final debe tener exactamente 4 digitos seguidos de una letra.");
            }

            string parteNumerica = codigoFinal.substr(0, 4);
            char letra = codigoFinal[4];

            // Validar numeros y letra
            if (!all_of(parteNumerica.begin(), parteNumerica.end(), ::isdigit)) {
                throw invalid_argument("Los primeros 4 caracteres del codigo final deben ser digitos.");
            }
            if (!isalpha(letra)) {
                throw invalid_argument("El ultimo caracter del codigo final debe ser una letra.");
            }

            // Convertir letra a mayuscula si esta en minuscula
            letra = toupper(letra);
            codigoFinal = parteNumerica + letra;

            // Construir el codigo completo
            codigoCompleto = "RES-" + numeroReserva + "-" + codigoFinal;

            formatoValido = true; // Validacion exitosa
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << endl;
            cout << "Desea intentarlo de nuevo? (S/N): ";
            char opcion;
            cin >> opcion;
            if (tolower(opcion) != 's') {
                cout << "Cancelando anulacion de reservacion.\n";
                return;
            }
        }
    }

    // Proceder con la anulacion
    bool exito = sistema->eliminarReservacionPorCodigo(codigoCompleto, this->getDocumento());

    if (exito) {
        cout << "Reservacion '" << codigoCompleto << "' anulada exitosamente.\n";
    } else {
        cout << "No se encontro una reservacion con el codigo '" << codigoCompleto << "' asociada a su documento.\n";
    }
}
