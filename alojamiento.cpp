#include "alojamiento.h"

#include <sstream>
#include <stdexcept>

using namespace std;

// Constructor por defecto
Alojamiento::Alojamiento() : codigo(""), nombre(""), documento(""), departamento(""), municipio(""),
    tipo('C'), direccion(""), precio(0.0f), amenidades(nullptr), capacidad(0), numAmenidades(0){}

// Constructor principal
Alojamiento::Alojamiento(const string& cod, const string& nom, const string& docAnf, const string& dep,
                         const string& mun, char tip, const string& dir,float prec, const string& ameStr) :
    codigo(cod), nombre(nom), documento(docAnf), departamento(dep), municipio(mun),
    tipo(tip), direccion(dir), precio(prec), capacidad(10), numAmenidades(0) {

    // Validaciones
    if (tipo != 'C' && tipo != 'A') {
        throw invalid_argument("Tipo de alojamiento invalido. Use 'C' (Casa) o 'A' (Apartamento)");
    }
    if (precio < 0) {
        throw invalid_argument("El precio no puede ser negativo");
    }

    // Inicializar array de amenidades
    amenidades = new string[capacidad];

    // Procesar string de amenidades
    procesarAmenidades(ameStr);
}

// Constructor de copia
Alojamiento::Alojamiento(const Alojamiento& otro) : codigo(otro.codigo), nombre(otro.nombre),
    documento(otro.documento), departamento(otro.departamento), municipio(otro.municipio),
    tipo(otro.tipo), direccion(otro.direccion), precio(otro.precio),
    capacidad(otro.capacidad), numAmenidades(otro.numAmenidades) {

    // Copia profunda de amenidades
    amenidades = new string[capacidad];
    for (int i = 0; i < numAmenidades; i++) {
        amenidades[i] = otro.amenidades[i];
    }
}

// Operador de asignación
Alojamiento& Alojamiento::operator=(const Alojamiento& otro) {
    if (this != &otro) {
        // Liberar memoria existente
        delete[] amenidades;

        // Copiar miembros simples
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

        // Copia profunda de amenidades
        amenidades = new string[capacidad];
        for (int i = 0; i < numAmenidades; i++) {
            amenidades[i] = otro.amenidades[i];
        }
    }
    return *this;
}

// Destructor
Alojamiento::~Alojamiento() {
    delete[] amenidades;
}

void Alojamiento::agregarAmenidad(const string& amenidad) {
    if (numAmenidades >= capacidad) {
        // Redimensionar
        capacidad *= 2;
        string* nuevo = new string[capacidad];

        // Copiar existentes
        for (int i = 0; i < numAmenidades; i++) {
            nuevo[i] = amenidades[i];
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
    return amenidades[index];
}

void Alojamiento::procesarAmenidades(const string& amenidadesStr) {
    stringstream ss(amenidadesStr);
    string amenidad;

    while (getline(ss, amenidad, ',')) {
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
