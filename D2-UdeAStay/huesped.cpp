#include "huesped.h"
#include "sistema.h"
#include "alojamiento.h"
#include "anfitrion.h"
#include "fecha.h"
#include "medicionrecursos.h"

#include <cstring>
#include <iostream>
#include <iomanip>
#include <cstdlib>

/**
 * @brief Constructor por defecto de la clase Huesped.
 *
 * Inicializa todos los atributos con valores predeterminados: cadenas vacías,
 * valores numéricos en cero, y punteros nulos.
 */
Huesped::Huesped() : nombre(""), documento(""), clave(""), antiguedad(0), puntuacion(0.0f),
reservaciones(nullptr), numReservaciones(0), capacidadReservaciones(0){}

/**
 * @brief Constructor con parámetros para inicializar un huésped con datos específicos.
 *
 * Realiza una validación básica de los datos proporcionados.
 *
 * @param nom Nombre completo del huésped.
 * @param doc Documento de identidad del huésped.
 * @param clv Clave o contraseña del huésped.
 * @param ant Antigüedad en meses del huésped dentro de la plataforma.
 * @param punt Puntuación del huésped (valor entre 0.0 y 5.0).
 *
 * @throws std::invalid_argument Si algún campo obligatorio está vacío o fuera de rango.
 */
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

/**
 * @brief Constructor de copia de la clase Huesped.
 *
 * Crea una copia profunda del objeto, incluyendo las reservaciones.
 *
 * @param otro Objeto Huesped del cual se realizará la copia.
 */
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

/**
 * @brief Destructor de la clase Huesped.
 *
 * Libera la memoria dinámica utilizada por el arreglo de reservaciones.
 */
Huesped::~Huesped() {
    delete[] reservaciones;
    reservaciones = nullptr;
}

/**
 * @brief Operador de asignación para la clase Huesped.
 *
 * Realiza una asignación profunda de los atributos de otro objeto Huesped.
 * Libera la memoria dinámica existente antes de copiar los nuevos datos.
 *
 * @param otro Objeto Huesped desde el cual se copiarán los valores.
 * @return Referencia al objeto Huesped actual, ya modificado.
 */
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

/**
 * @brief Expande la capacidad del arreglo dinámico de reservaciones.
 *
 * Duplica la capacidad actual (o la inicializa en 2 si es 0),
 * copia todas las reservaciones al nuevo arreglo, libera la memoria anterior
 * y actualiza el puntero y la capacidad.
 *
 * También actualiza las métricas de memoria e iteración usando las funciones
 * agregarMemoria e incrementarIteracion.
 */
void Huesped::expandirCapacidadReservaciones() {
    int nuevaCapacidad = (capacidadReservaciones == 0) ? 2 : capacidadReservaciones * 2;
    Reservacion* nuevoArray = new Reservacion[nuevaCapacidad];
    agregarMemoria(sizeof(Reservacion) * nuevaCapacidad);

    for (int i = 0; i < numReservaciones; i++) {
        incrementarIteracion();
        nuevoArray[i] = reservaciones[i];
    }

    delete[] reservaciones;
    reservaciones = nuevoArray;
    capacidadReservaciones = nuevaCapacidad;
}

/**
 * @brief Solicita al huésped una fecha de entrada válida.
 *
 * Muestra un mensaje en consola pidiendo la fecha en formato DD/MM/AAAA.
 * Valida que el formato sea correcto y que la fecha no haya pasado.
 *
 * @return Objeto Fecha con la fecha ingresada por el usuario.
 */
Fecha Huesped::solicitarFechaEntrada() {
    incrementarIteracion();
    Fecha fechaEntrada;
    string fechaStr;

    while (true) {
        cout << "Ingrese fecha de entrada (DD/MM/AAAA): ";
        cin >> fechaStr;

        fechaEntrada = Fecha(fechaStr);
        if (!fechaEntrada.esValida()) {
            incrementarIteracion();
            cout << "Fecha invalida. Formato correcto: DD/MM/AAAA\n";
            continue;
        }
        if (fechaEntrada.yaPaso()) {
            incrementarIteracion();
            cout << "La fecha ya ha pasado. Ingrese una fecha futura.\n";
            continue;
        }
        break;
    }
    return fechaEntrada;
}

/**
 * @brief Solicita al huésped el nombre del municipio de destino.
 *
 * Pide al usuario que ingrese un municipio no vacío.
 * En caso de entrada vacía, muestra un mensaje y vuelve a solicitarlo
 * mediante una llamada recursiva.
 *
 * @return Cadena de texto con el municipio ingresado.
 */
string Huesped::solicitarMunicipio() {
    incrementarIteracion();
    string municipio;
    cout << "Ingrese municipio de destino: ";
    cin.ignore();
    getline(cin, municipio);

    if (municipio.empty()) {
        incrementarIteracion();
        cout << "El municipio no puede estar vacio.\n";
        return solicitarMunicipio(); // Recursion para reintentar
    }
    return municipio;
}

/**
 * @brief Solicita al huésped la cantidad de noches para su estadía.
 *
 * Valida que la entrada sea un número entero positivo. Si no lo es, vuelve a pedirla.
 *
 * @return Número entero con la cantidad de noches.
 */
int Huesped::solicitarCantidadNoches() {
    incrementarIteracion();
    int noches;
    while (true) {
        incrementarIteracion();
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

/**
 * @brief Solicita al huésped un precio máximo opcional para la búsqueda de alojamientos.
 *
 * Pregunta si desea limitar el precio. Si responde afirmativamente ('s' o 'S'),
 * solicita un número decimal mayor o igual a cero. Si no desea limitarlo, retorna -1.
 *
 * @return Precio máximo como número flotante, o -1 si no se desea establecer un límite.
 */
float Huesped::solicitarPrecioMaximo() {
    incrementarIteracion();
    char opc;
    cout << "Desea ingresar un precio maximo? (s/n): ";
    cin >> opc;

    if (opc != 's' && opc != 'S') return -1; // Sin limite

    float precio;
    while (true) {
        incrementarIteracion();
        cout << "Ingrese precio maximo: ";
        string entrada;
        cin >> entrada;
        try {
            incrementarIteracion();
            precio = stof(entrada);
            if (precio < 0) throw invalid_argument("Negativo");
            break;
        } catch (...) {
            incrementarIteracion();
            cout << "Precio invalido. Ingrese un numero valido mayor o igual a 0.\n";
        }
    }
    return precio;
}

/**
 * @brief Solicita al huésped una puntuación mínima opcional para filtrar alojamientos.
 *
 * Pregunta si desea ingresar una puntuación mínima. Si responde 's' o 'S',
 * solicita un valor flotante entre 0.0 y 5.0. Si no desea limitarla, retorna -1.
 *
 * @return Puntuación mínima como número flotante, o -1 si no se desea establecer un límite.
 */
float Huesped::solicitarPuntuacionMinima() {
    incrementarIteracion();
    char opc;
    cout << "Desea ingresar una puntuacion minima? (s/n): ";
    cin >> opc;

    if (opc != 's' && opc != 'S') return -1; // Sin limite
    incrementarIteracion();

    float puntuacion;
    while (true) {
        incrementarIteracion();
        cout << "Ingrese puntuacion minima (0.0 - 5.0): ";
        string entrada;
        cin >> entrada;
        try {
            incrementarIteracion();
            puntuacion = stof(entrada);
            if (puntuacion < 0.0f || puntuacion > 5.0f)
                throw out_of_range("Fuera de rango");
            break;
        } catch (...) {
            incrementarIteracion();
            cout << "Puntuacion invalida. Debe estar entre 0.0 y 5.0.\n";
        }
    }
    return puntuacion;
}

/**
 * @brief Solicita al huésped seleccionar un método de pago válido.
 *
 * Muestra las opciones disponibles (Tarjeta de crédito o PSE) y valida la entrada.
 * Repite la solicitud hasta recibir una opción válida.
 *
 * @return Caracter que representa el método de pago ('T' para tarjeta, 'P' para PSE).
 */
char Huesped::solicitarMetodoPago() {
    incrementarIteracion();
    char metodo;
    while (true) {
        incrementarIteracion();
        cout << "\nMetodos de pago disponibles:\n";
        cout << "[T] Tarjeta de credito\n";
        cout << "[P] PSE\n";
        cout << "Seleccione metodo de pago: ";
        cin >> metodo;

        metodo = toupper(metodo);
        if (metodo == 'T' || metodo == 'P') {
            incrementarIteracion();
            break;
        }
        cout << "Opcion invalida. Intente nuevamente.\n";
    }
    return metodo;
}

/**
 * @brief Solicita al huésped ingresar anotaciones especiales opcionales.
 *
 * Pregunta si desea agregar anotaciones, y en caso afirmativo, permite
 * ingresar texto multilinea.
 *
 * @return Cadena con las anotaciones ingresadas, o cadena vacía si no se ingresaron.
 */
string Huesped::solicitarAnotaciones() {
    incrementarIteracion();
    string anotaciones;
    char opc;
    cout << "Desea agregar anotaciones especiales? (s/n): ";
    cin >> opc;

    if (opc == 's' || opc == 'S') {
        incrementarIteracion();
        cout << "Ingrese sus anotaciones: ";
        cin.ignore();
        getline(cin, anotaciones);
    }
    return anotaciones;
}

/**
 * @brief Permite al huésped seleccionar un alojamiento de un arreglo de alojamientos disponibles.
 *
 * Muestra los alojamientos con un índice numerado, y solicita al usuario seleccionar uno.
 * El usuario puede cancelar la selección ingresando 0.
 *
 * @param disponibles Arreglo de alojamientos disponibles.
 * @param cantidad Número de alojamientos disponibles.
 * @return Índice del alojamiento seleccionado (base 0), o -1 si se cancela.
 */
int Huesped::seleccionarAlojamiento(Alojamiento* disponibles, int cantidad) {
    incrementarIteracion();
    // Mostrar opciones
    cout << "\n=== ALOJAMIENTOS DISPONIBLES ===\n";
    for (int i = 0; i < cantidad; ++i) {
        incrementarIteracion();
        cout << "[" << (i + 1) << "] ";
        disponibles[i].mostrar();
        cout << "\n";
    }

    // Seleccion
    int seleccion;
    while (true) {
        incrementarIteracion();
        cout << "Seleccione una opcion (1-" << cantidad << ") o 0 para cancelar: ";
        cin >> seleccion;

        if (seleccion == 0) return -1; // Cancelar
        if (seleccion >= 1 && seleccion <= cantidad) {
            incrementarIteracion();
            return seleccion - 1; // indice basado en 0
        }
        cout << "Opcion invalida.\n";
    }
}

/**
 * @brief Método principal para que el huésped realice una reserva de alojamiento.
 *
 * Este método gestiona todo el proceso de reserva:
 * - Solicita datos al usuario (fecha, municipio, noches, precio máximo y puntuación mínima).
 * - Busca alojamientos disponibles en el sistema con esos criterios.
 * - Permite al usuario seleccionar un alojamiento.
 * - Crea una nueva reservación.
 * - Muestra la confirmación de la reserva.
 *
 * Si no hay alojamientos disponibles, pregunta si desea intentar con otros criterios.
 * La función termina si el usuario cancela o finaliza la reserva con éxito.
 *
 * @param sistema Puntero al sistema que gestiona alojamientos y reservaciones.
 */
void Huesped::ReservarAlojamiento(Sistema* sistema) {
    incrementarIteracion();
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
            incrementarIteracion();
            char opcion;
            cout << "\nNo hay alojamientos disponibles con esos criterios.\n";
            cout << "Desea intentar con otros criterios? (s/n): ";
            cin >> opcion;

            if (tolower(opcion) == 'n') {
                incrementarIteracion();
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

/**
 * @brief Muestra en consola el comprobante de confirmación de una reservación.
 *
 * Despliega información relevante de la reservación y del alojamiento seleccionado,
 * incluyendo fechas, monto, método de pago y anotaciones especiales si existen.
 *
 * @param reservacion Referencia constante a la reservación confirmada.
 * @param alojamiento Referencia constante al alojamiento reservado.
 */
void Huesped::mostrarConfirmacionReservacion(const Reservacion& reservacion,
                                             const Alojamiento& alojamiento) {
    incrementarIteracion();
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

/**
 * @brief Muestra todas las reservaciones del huésped en consola, separadas en futuras y pasadas.
 *
 * Este método:
 * - Muestra el nombre y documento del huésped.
 * - Solicita al sistema las reservaciones asociadas a este huésped.
 * - Si no hay reservaciones, informa al usuario y espera que presione Enter.
 * - Si hay reservaciones, muestra primero las futuras y luego las pasadas, con detalle.
 * - Al final, muestra el total de reservaciones y espera que el usuario presione Enter para continuar.
 *
 * @param sistema Puntero al sistema que gestiona las reservaciones y alojamientos.
 */
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

/**
 * @brief Permite al huésped anular una reservación especificando su número y código final.
 *
 * Este método solicita al usuario el número de reservación y el código final, validando
 * su formato. El código final debe tener exactamente 5 caracteres: los primeros 4 dígitos
 * y una letra (mayúscula o minúscula, que se convierte a mayúscula).
 *
 * La función construye el código completo de la reservación en el formato:
 * "RES-<numeroReserva>-<codigoFinal>", y luego intenta eliminar la reservación en el sistema
 * asociada al documento del huésped.
 *
 * Si la validación falla, ofrece la opción de reintentar o cancelar la operación.
 *
 * @param sistema Puntero al sistema que administra las reservaciones.
 */
void Huesped::anularReservacion(Sistema* sistema) {
    incrementarIteracion();
    string numeroReserva;
    string codigoFinal;
    string codigoCompleto;
    bool formatoValido = false;

    while (!formatoValido) {
        try {
            incrementarIteracion();
            cout << "Ingrese el numero de la reservacion: ";
            cin >> numeroReserva;

            // Validar que sea numerico
            if (numeroReserva.empty() || !all_of(numeroReserva.begin(), numeroReserva.end(), ::isdigit)) {
                incrementarIteracion();
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
                incrementarIteracion();
                throw invalid_argument("Los primeros 4 caracteres del codigo final deben ser digitos.");
            }
            if (!isalpha(letra)) {
                incrementarIteracion();
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
            incrementarIteracion();
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
