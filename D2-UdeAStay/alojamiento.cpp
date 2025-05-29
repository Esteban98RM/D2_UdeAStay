#include "alojamiento.h"
#include "reservacion.h"
#include "anfitrion.h"
#include "medicionrecursos.h"

#include <sstream>
#include <stdexcept>

using namespace std;

/**
 * @brief Constructor por defecto de la clase Alojamiento.
 *
 * Inicializa los atributos con valores predeterminados.
 * Crea un arreglo de amenidades vacío (puntero a nullptr).
 */
Alojamiento::Alojamiento() : codigo(""), nombre(""), documento(""), departamento(""),
    municipio(""), tipo('C'), direccion(""), precio(0.0f),
    amenidades(nullptr), capacidad(0), numAmenidades(0){}

/**
 * @brief Constructor principal de la clase Alojamiento.
 *
 * @param cod Código del alojamiento.
 * @param nom Nombre del alojamiento.
 * @param docAnf Documento del anfitrión.
 * @param dep Departamento donde se encuentra el alojamiento.
 * @param mun Municipio donde se encuentra el alojamiento.
 * @param tip Tipo de alojamiento ('C' para casa, 'A' para apartamento).
 * @param dir Dirección del alojamiento.
 * @param prec Precio por noche.
 * @param ameStr Cadena de amenidades separadas por comas.
 *
 * @throws std::invalid_argument si el tipo es inválido o el precio es negativo.
 */
Alojamiento::Alojamiento(const string& cod, const string& nom, const string& docAnf,
                         const string& dep, const string& mun, char tip,
                         const string& dir, float prec, const string& ameStr) :
    codigo(cod), nombre(nom), documento(docAnf), departamento(dep),
    municipio(mun), tipo(tip), direccion(dir), precio(prec),
    capacidad(10), numAmenidades(0) {

    if (tipo != 'C' && tipo != 'A') {
        throw invalid_argument("Tipo de alojamiento invalido");
    }
    if (precio < 0) {
        throw invalid_argument("El precio no puede ser negativo");
    }

    amenidades = new string[capacidad];
    agregarMemoria(sizeof(string) * capacidad);

    procesarAmenidades(ameStr);
}

/**
 * @brief Constructor de copia.
 *
 * Crea una copia profunda del objeto Alojamiento recibido.
 *
 * @param otro Objeto Alojamiento que se desea copiar.
 */
Alojamiento::Alojamiento(const Alojamiento& otro) :
    codigo(otro.codigo), nombre(otro.nombre), documento(otro.documento),
    departamento(otro.departamento), municipio(otro.municipio),
    tipo(otro.tipo), direccion(otro.direccion), precio(otro.precio),
    capacidad(otro.capacidad), numAmenidades(otro.numAmenidades) {

    amenidades = new string[capacidad];
    agregarMemoria(sizeof(string) * capacidad);

    for (int i = 0; i < numAmenidades; i++) {
        amenidades[i] = otro.amenidades[i];
        incrementarIteracion();
    }
}

/**
 * @brief Operador de asignación.
 *
 * Realiza una copia profunda de otro objeto Alojamiento, liberando previamente los recursos actuales.
 *
 * @param otro Objeto Alojamiento que se desea asignar.
 * @return Referencia al objeto actual.
 */
Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this != &otro) {

        agregarMemoria(-sizeof(string) * capacidad);
        delete[] amenidades;

        codigo = otro.codigo;
        nombre = otro.nombre;
        documento = otro.documento;
        departamento = otro.departamento;
        municipio = otro.municipio;
        tipo = otro.tipo;
        direccion = otro.direccion;
        precio = otro.precio;
        capacidad = otro.capacidad;
        numAmenidades = otro.numAmenidades;

        amenidades = new string[capacidad];
        agregarMemoria(sizeof(string) * capacidad);

        for (int i = 0; i < numAmenidades; i++) {
            amenidades[i] = otro.amenidades[i];
            incrementarIteracion();
        }

    }
    return *this;
}

/**
 * @brief Destructor de la clase Alojamiento.
 *
 * Libera la memoria dinámica asociada al arreglo de amenidades.
 */
Alojamiento::~Alojamiento() {
    //inicializarContador();
    agregarMemoria(-sizeof(string) * capacidad);
    delete[] amenidades;
}

/**
 * @brief Agrega una nueva amenidad al alojamiento.
 *
 * Si se alcanza la capacidad del arreglo, se duplica su tamaño antes de agregar.
 * También actualiza el conteo de iteraciones y memoria usada.
 *
 * @param amenidad Nombre de la amenidad a agregar.
 */
void Alojamiento::agregarAmenidad(const string& amenidad) {
    incrementarIteracion();
    if (numAmenidades >= capacidad) {
        // Medimos memoria antes y después de redimensionar
        agregarMemoria(-sizeof(string) * capacidad);

        // Redimensionar
        capacidad *= 2;
        string* nuevo = new string[capacidad];
        agregarMemoria(sizeof(string) * capacidad);

        // Copiar existentes
        for (int i = 0; i < numAmenidades; i++) {
            nuevo[i] = amenidades[i];
            incrementarIteracion();
        }

        delete[] amenidades;
        amenidades = nuevo;
    }

    // Agregar nueva amenidad
    amenidades[numAmenidades++] = amenidad;
}

/**
 * @brief Obtiene una amenidad específica del alojamiento.
 *
 * @param index Índice de la amenidad a obtener.
 * @return Referencia constante a la cadena de la amenidad.
 *
 * @throws std::out_of_range si el índice es inválido.
 */
const string& Alojamiento::obtenerAmenidad(int index) const {
    if (index < 0 || index >= numAmenidades) {
        throw out_of_range("Indice de amenidad invalido");
    }
    incrementarIteracion();
    return amenidades[index];
}

/**
 * @brief Procesa una cadena de texto con amenidades separadas por comas.
 *
 * Divide la cadena, limpia espacios, convierte a minúsculas y agrega cada amenidad.
 *
 * @param amenidadesStr Cadena de amenidades separadas por comas.
 */
void Alojamiento::procesarAmenidades(const string& amenidadesStr) {
    stringstream ss(amenidadesStr);
    string amenidad;

    while (getline(ss, amenidad, ',')) {
        incrementarIteracion();
        // Eliminar espacios al inicio y final
        amenidad.erase(0, amenidad.find_first_not_of(' '));
        amenidad.erase(amenidad.find_last_not_of(' ') + 1);

        // Convertir a minusculas para estandarizar
        transform(amenidad.begin(), amenidad.end(), amenidad.begin(), ::tolower);

        if (!amenidad.empty()) {
            agregarAmenidad(amenidad);
        }
    }
}

/**
 * @brief Verifica si el alojamiento está disponible en un rango de fechas dado.
 *
 * Revisa si el alojamiento no tiene reservas que se superpongan con el rango desde `entrada`
 * hasta `entrada + noches`. Compara las fechas de entrada y salida de todas las reservas.
 *
 * @param a Objeto Alojamiento al que se desea verificar la disponibilidad.
 * @param entrada Fecha de entrada deseada.
 * @param noches Cantidad de noches de la estancia.
 * @param reservas Arreglo de reservas existentes.
 * @param numReservas Número de reservas en el arreglo.
 *
 * @return true si el alojamiento está disponible en el rango indicado, false si ya está reservado.
 */
bool Alojamiento::estaDisponible(const Alojamiento& a, const Fecha& entrada,
                                 int noches, const Reservacion* reservas,
                                 int numReservas) const {
    Fecha salida = entrada;
    salida.sumarDias(noches);

    for (int i = 0; i < numReservas; ++i) {
        incrementarIteracion();
        if (reservas[i].getCodigoAlojamiento() != a.getCodigo())
            continue;

        Fecha inicioReserva = reservas[i].getFechaEntrada();
        Fecha finReserva = inicioReserva;
        finReserva.sumarDias(reservas[i].getDuracion());

        if (entrada < finReserva && salida > inicioReserva) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Muestra la información del alojamiento en consola.
 *
 * Incluye el nombre, municipio, precio por noche, código del alojamiento
 * y la lista de amenidades. Si no hay amenidades, se indica explícitamente.
 */
void Alojamiento::mostrar() const {
    cout << "Nombre: " << nombre << endl;
    cout << "Municipio: " << municipio << endl;
    cout << "Precio por noche: $" << precio << endl;

    cout << "Amenidades: ";
    if (numAmenidades == 0) {
        cout << "Ninguna";
    } else {
        for (int i = 0; i < numAmenidades; ++i) {
            incrementarIteracion();
            cout << amenidades[i];
            if (i < numAmenidades - 1) cout << ", ";
        }
    }
    cout << endl;

    cout << "Codigo: " << codigo << endl;
    cout << "---------------------------" << endl;
}

