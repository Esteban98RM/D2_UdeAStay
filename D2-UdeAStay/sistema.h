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

    int ultimoIdReservacion;
    bool esReservacionDelAnfitrionEnRango(const Reservacion& reserva,
                                          const string& documentoAnfitrion,
                                          const Fecha& fechaInicio,
                                          const Fecha& fechaFin,
                                          const Alojamiento* alojamientos,
                                          int totalAlojamientos) const;

    friend class GestionArchivos;

public:

    //Constructor
    Sistema();
    Sistema(const string& doc);

    //Destructor
    ~Sistema();

    //Getters
    Alojamiento* getAlojamientos() const { return alojamientos; }
    int getNumAlojamientos() const { return numAlojamientos; }
    Anfitrion* getAnfitriones() const { return anfitriones; }
    int getNumAnfitriones() const { return numAnfitriones; }
    Huesped* getHuespedes() const { return huespedes; }
    int getNumHuespedes() const { return numHuespedes; }
    const Reservacion* getReservaciones() const { return reservaciones; }
    int getNumReservaciones() const { return numReservaciones; }


    // Sobrecarga de operador
    bool operator==(const Sistema& otro) const;
    // Sobrecarga para huesped
    Reservacion* obtenerReservaciones(const string& documentoHuesped,
                                      int& totalFuturas,
                                      int& totalPasadas) const;

    // Sobrecarga para anfitrion
    Reservacion* obtenerReservaciones(const string& documentoAnfitrion,
                                      const Fecha& fechaInicio,
                                      const Fecha& fechaFin,
                                      int& totalEncontradas) const;

    // Metodos
    void cargarDatos();
    //bool guardarDatos();
    bool autenticarUsuario(const string& doc, const string& clave, string& tipoRol);
    bool iniciarSesion(const string& documento, const string& clave);
    void mostrarMenuHuesped(Huesped& huesped);
    void mostrarMenuAnfitrion(Anfitrion& anfitrion);

    Alojamiento* filtrarAlojamientosDisponibles(const string& municipio, float precioMax,
    float puntuacionMin, int& cantidadFiltrada);

    // Metodo unificado para buscar alojamientos disponibles
    Alojamiento* buscarAlojamientosDisponibles(const string& municipio,
                                               const Fecha& fechaEntrada,
                                               int noches,
                                               float precioMax,
                                               float puntuacionMin,
                                               int& cantidadEncontrada);

    // Crear reservacion completa
    Reservacion* crearReservacion(const Alojamiento& alojamiento,const Fecha& fechaEntrada,
    int noches, Huesped* huesped);

    // Metodo para buscar alojamiento por codigo especifico
    Alojamiento* buscarAlojamientoPorCodigo(const string& codigo);

    // Validar que huesped no tenga conflictos de reserva
    bool validarDisponibilidadHuesped(const string& documentoHuesped, const Fecha& fechaEntrada,
    int noches);

    bool eliminarReservacionPorCodigo(const string& codigo, const string& documento);

    // Generar codigo unico con estructura especifica
    string generarCodigoReservacion();

    // Agregar reservacion al sistema y guardar en archivo
    void agregarReservacion(const Reservacion& reservacion);

    // Metodos para el corte historico
    string obtenerUltimaFechaCorteHistorico();
    void procesarActualizacionHistorico(const Fecha& fechaCorte);
    void mostrarEstadisticasHistorico(const Fecha& fechaCorte, int finalizadas, int activas, int enCurso);
};

#endif // SISTEMA_H
