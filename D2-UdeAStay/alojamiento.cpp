#include "alojamiento.h"
#include "reservacion.h"
#include "anfitrion.h"
#include "medicionrecursos.h"

#include <sstream>
#include <stdexcept>

using namespace std;

// Constructor por defecto
Alojamiento::Alojamiento() : codigo(""), nombre(""), documento(""), departamento(""),
    municipio(""), tipo('C'), direccion(""), precio(0.0f),
    amenidades(nullptr), capacidad(0), numAmenidades(0){}

// Constructor principal
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

// Constructor de copia
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

    //mostrarEstadisticasRecursos();
}

// Operador de asignacion
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

// Destructor
Alojamiento::~Alojamiento() {
    //inicializarContador();
    agregarMemoria(-sizeof(string) * capacidad);
    delete[] amenidades;
}

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

const string& Alojamiento::obtenerAmenidad(int index) const {
    if (index < 0 || index >= numAmenidades) {
        throw out_of_range("Indice de amenidad invalido");
    }
    incrementarIteracion();
    return amenidades[index];
}

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

