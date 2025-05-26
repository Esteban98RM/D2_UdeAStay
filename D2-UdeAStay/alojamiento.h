

    // string* fechasReservadas;
    // int capacidadFechas;
    // int numFechasReservadas;


#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include "anfitrion.h"

#include "fecha.h"
#include <string>

class Reservacion;

class Alojamiento {
private:
    string codigo;
    string nombre;
    string documento;
    string departamento;
    string municipio;
    char tipo;
    string direccion;
    float precio;

    string* amenidades;
    int capacidad;
    int numAmenidades;

    //Anfitrion* anfitrion;

public:
    Alojamiento();
    Alojamiento(const string& cod, const string& nom, const string& docAnf, const string& dep,
                const string& mun, char tip, const string& dir, float prec, const string& ameStr);
    Alojamiento(const Alojamiento& otro);
    Alojamiento& operator=(const Alojamiento& otro);
    ~Alojamiento();

    // Métodos para amenidades
    void agregarAmenidad(const string& amenidad);
    const string& obtenerAmenidad(int index) const;
    int getNumAmenidades() const { return numAmenidades; }
    void procesarAmenidades(const string& amenidadesStr);

    // Getters
    const string& getCodigo() const { return codigo; }
    const string& getNombre() const { return nombre; }
    char getTipo() const { return tipo; }
    float getPrecio() const { return precio; }
    const string getMunicipio() const { return municipio; }
    const string getDocumentoAnfitrion() const { return documento; }

    // Métodos principales
    bool estaDisponible(const Alojamiento& a, const Fecha& entrada, int noches,
                        const Reservacion* reservaciones, int numReservas) const;
    void mostrar() const;
};

#endif // ALOJAMIENTO_H
