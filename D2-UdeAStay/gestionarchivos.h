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

    // Nuevos metodos para guardar
    void guardarReservacion(const Reservacion& reservacion);
    void actualizarArchivoReservaciones(Reservacion* reservaciones, int cantidad);
    bool guardarReservacionesHistorico(Reservacion* reservaciones, int cantidad,
    const string& fechaCorte);

    // Nuevos metodos para el ID
    int cargarUltimoIdReservacion();
    void guardarUltimoIdReservacion(int id);

    // Nuevos metodos para manejo de fecha de corte historico
    string cargarUltimaFechaCorteHistorico();
    void guardarUltimaFechaCorteHistorico(const string& fecha);

};

#endif // GESTIONARCHIVOS_H
