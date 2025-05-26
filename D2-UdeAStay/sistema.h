#ifndef SISTEMA_H
#define SISTEMA_H

#include "fecha.h"
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
    GestionArchivos* gestionArchivos;

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

    //Getters
    Alojamiento* getAlojamientos() const { return alojamientos; }
    int getNumAlojamientos() const { return numAlojamientos; }
    Anfitrion* getAnfitriones() const { return anfitriones; }
    int getNumAnfitriones() const { return numAnfitriones; }
    const Reservacion* getReservaciones() const { return reservaciones; }
    int getNumReservaciones() const { return numReservaciones; }


    // Sobrecarga de operador
    bool operator==(const Sistema& otro) const;

    // Metodos
    void cargarDatos();
    bool guardarDatos();
    bool autenticarUsuario(const string& doc, const string& clave, string& tipoRol);
    void iniciarSesion(const string& documento, const string& clave);
    void mostrarMenuHuesped(Huesped& huesped);
    void mostrarMenuAnfitrion(Anfitrion& anfitrion);

    Alojamiento* filtrarAlojamientosDisponibles(const string& municipio, float precioMax,
    float puntuacionMin, int& cantidadFiltrada);

    // Método unificado para buscar alojamientos disponibles
    Alojamiento* buscarAlojamientosDisponibles(const string& municipio,
                                               const Fecha& fechaEntrada,
                                               int noches,
                                               float precioMax,
                                               float puntuacionMin,
                                               int& cantidadEncontrada);

    // Crear reservación completa
    Reservacion* crearReservacion(const Alojamiento& alojamiento,
                                  const Fecha& fechaEntrada,
                                  int noches,
                                  Huesped* huesped);

    // Método para buscar alojamiento por código específico
    Alojamiento* buscarAlojamientoPorCodigo(const string& codigo);

    // Validar que huésped no tenga conflictos de reserva
    bool validarDisponibilidadHuesped(const string& documentoHuesped,
                                      const Fecha& fechaEntrada,
                                      int noches);

    // Generar código único con estructura específica
    string generarCodigoReservacion();

    // Agregar reservación al sistema y guardar en archivo
    void agregarReservacion(const Reservacion& reservacion);
};

#endif // SISTEMA_H
