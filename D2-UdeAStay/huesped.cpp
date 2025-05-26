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
Huesped::Huesped() : documento(""), clave(""), antiguedad(0), puntuacion(0.0f),
reservaciones(nullptr), numReservaciones(0), capacidadReservaciones(0){}

// Constructor con parametros
Huesped::Huesped(const string& doc, const string& clv, int ant, float punt) :
    documento(doc), clave(clv), antiguedad(ant), puntuacion(punt),
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
}

Huesped& Huesped::operator=(const Huesped& otro) {
    if (this != &otro) {
        delete[] reservaciones;

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
    cout << "¿Desea ingresar un precio maximo? (s/n): ";
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
    cout << "¿Desea ingresar una puntuacion minima? (s/n): ";
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
        cout << "[T] Tarjeta\n";
        cout << "[P] PayPal\n";
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
        cout << "No hay alojamientos disponibles con esos criterios.\n";
        return;
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
}

void Huesped::mostrarConfirmacionReservacion(const Reservacion& reservacion,
                                             const Alojamiento& alojamiento) {
    // Crear fechas para formateo completo
    Fecha fechaEntrada(reservacion.getFechaEntrada());
    Fecha fechaSalida = fechaEntrada.calcularFechaFinal(reservacion.getDuracion());

    cout << "\n" << string(60, '=') << "\n";
    cout << "            COMPROBANTE DE CONFIRMACIoN\n";
    cout << string(60, '=') << "\n";
    cout << "Codigo de reserva: " << reservacion.getCodigo() << "\n";
    cout << "Nombre del usuario: " << this->getDocumento() << "\n"; // Asumiendo que tienes metodo getNombre()
    cout << "Codigo del alojamiento: " << alojamiento.getCodigo() << "\n";
    cout << "Alojamiento: " << alojamiento.getNombre() << "\n";
    cout << "Municipio: " << alojamiento.getMunicipio() << "\n";
    cout << "\nFECHAS DE ESTADiA:\n";
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

// Metodo para reservar por codigo especifico
void Huesped::ReservarAlojamientoPorCodigo(Sistema* sistema) {
    string codigoAlojamiento;
    cout << "Ingrese el codigo del alojamiento: ";
    cin >> codigoAlojamiento;

    // Buscar alojamiento
    Alojamiento* alojamiento = sistema->buscarAlojamientoPorCodigo(codigoAlojamiento);
    if (!alojamiento) {
        cout << "No se encontro un alojamiento con ese codigo.\n";
        return;
    }

    // Mostrar informacion del alojamiento
    cout << "\n=== ALOJAMIENTO ENCONTRADO ===\n";
    alojamiento->mostrar();

    char confirmar;
    cout << "\n¿Desea continuar con este alojamiento? (s/n): ";
    cin >> confirmar;
    if (confirmar != 's' && confirmar != 'S') {
        cout << "Reservacion cancelada.\n";
        return;
    }

    // Solicitar datos de reserva
    Fecha fechaEntrada = solicitarFechaEntrada();
    int noches = solicitarCantidadNoches();

    // Verificar disponibilidad especifica
    int nDisponibles = 0;
    Alojamiento disponibles[1] = {*alojamiento};
    Alojamiento* resultado = Reservacion::filtrarDisponiblesPorFecha(
        disponibles,
        1,
        fechaEntrada,
        noches,
        sistema->getReservaciones(),  // Ahora compatible con const
        sistema->getNumReservaciones(),
        nDisponibles
        );

    if (nDisponibles == 0) {
        cout << "El alojamiento no esta disponible en esas fechas.\n";
        return;
    }

    // Crear reservacion
    Reservacion* nuevaReservacion = sistema->crearReservacion(
        *alojamiento, fechaEntrada, noches, this
        );

    if (nuevaReservacion) {
        mostrarConfirmacionReservacion(*nuevaReservacion, *alojamiento);
    }
}
