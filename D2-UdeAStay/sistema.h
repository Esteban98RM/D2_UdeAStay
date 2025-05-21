#ifndef SISTEMA_H
#define SISTEMA_H

#include <string>

class Anfitrion;
class Huesped;
class Alojamiento;
class Reservacion;
class GestionArchivos;

using namespace std;

class Sistema
{
private:

    string documento;
    Anfitrion* anfitriones;
    Huesped* huespedes;
    Alojamiento* alojamientos;
    Reservacion* reservaciones;

    int numAnfitriones;
    int numHuespedes;
    int numAlojamientos;
    int numReservaciones;

    void liberarMemoria();

    friend class GestionArchivos;

public:

    //Constructor
    Sistema();
    Sistema(const string& doc);
    Sistema(const string& doc, Anfitrion* anf, Huesped* hue);

    //Destructor
    ~Sistema();

    // Sobrecarga de operador
    bool operator==(const Sistema& otro) const;

    // Metodos
    void cargarDatos();
    bool guardarDatos();
    bool autenticarUsuario(const string& doc, const string& clave, string& tipoRol);
};

#endif // SISTEMA_H
