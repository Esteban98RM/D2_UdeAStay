#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>

using namespace std;

class Alojamiento{
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

public:

    //Constructor
    Alojamiento();
    Alojamiento(const string& cod, const string& nom, const string& docAnf, const string& dep,
                const string& mun, char tip, const string& dir, float prec, const string& ameStr);

    //Constructor Copia
    Alojamiento(const Alojamiento& otro);

    //Sobrecarga de operador
    Alojamiento& operator=(const Alojamiento& otro);

    //Destructor
    ~Alojamiento();

    //Metodos para menejar las amenidades
    void agregarAmenidad(const string& amenidad);
    const string& obtenerAmenidad(int index) const;
    int getNumAmenidades() const { return numAmenidades; }
    void procesarAmenidades(const string& amenidadesStr);

    //Getters
    const string& getCodigo() const { return codigo; }
    const string& getNombre() const { return nombre; }
    char getTipo() const { return tipo; }
    float getPrecio() const { return precio; }


};

#endif // ALOJAMIENTO_H
