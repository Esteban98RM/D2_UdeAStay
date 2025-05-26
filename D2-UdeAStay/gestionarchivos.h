#ifndef GESTIONARCHIVOS_H
#define GESTIONARCHIVOS_H

#include <string>

class Anfitrion;
class Huesped;
class Alojamiento;
class Reservacion;

using namespace std;

class GestionArchivos {
private:

    int contarLineas(const string& nombreArchivo);
    string formatearMetodoPago(char metodo);

public:

    // Constructor
    GestionArchivos();

    //Metodos
    void cargarAnfitriones(Anfitrion*& anfitriones, int& cantidad);
    void cargarHuespedes(Huesped*& huespedes, int& cantidad);
    void cargarAlojamientos(Alojamiento*& alojamientos, int& cantidad);
    void cargarReservaciones(Reservacion*& reservaciones, int& cantidad);

    // Nuevos métodos para guardar
    void guardarReservacion(const Reservacion& reservacion);
    void actualizarArchivoReservaciones(Reservacion* reservaciones, int cantidad);

};

#endif // GESTIONARCHIVOS_H
