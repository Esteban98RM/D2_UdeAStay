#ifndef HUESPED_H
#define HUESPED_H

#include "reservacion.h"
#include "alojamiento.h"
#include "sistema.h"

#include <string>

using namespace std;

class Huesped {
private:

    Alojamiento aloj;

    string nombre;
    string documento;
    string clave;
    int antiguedad;
    float puntuacion;

    Reservacion* reservaciones;

    int numReservaciones;
    int capacidadReservaciones;

    void expandirCapacidadReservaciones();

public:

    Huesped();
    Huesped(const string& nom, const string& doc, const string& clv, int ant, float punt);
    Huesped(const Alojamiento& alojamiento) : aloj(alojamiento) {}
    Huesped(const Huesped& otro);

    ~Huesped();

    Huesped& operator=(const Huesped& otro);

    // Getters

    const Reservacion* getReservaciones() const { return reservaciones; }
    const string& getNombre() const { return nombre; }
    const string& getDocumento() const { return documento; }
    const string& getClave() const { return clave; }
    int getAntiguedad() const { return antiguedad; }
    int getNumReservaciones() const { return numReservaciones; }
    float getPuntuacion() const { return puntuacion; }

    //Metodos para reservas
    void ReservarAlojamiento(Sistema* sistema);
    void mostrarReservaciones(Sistema* sistema);
    void anularReservacion(Sistema* sistema);

    // Métodos de entrada de datos
    Fecha solicitarFechaEntrada();
    string solicitarMunicipio();
    int solicitarCantidadNoches();
    float solicitarPrecioMaximo();
    float solicitarPuntuacionMinima();
    char solicitarMetodoPago();
    string solicitarAnotaciones();

    // Métodos de selección
    int seleccionarAlojamiento(Alojamiento* disponibles, int cantidad);

    // Métodos de confirmación
    void mostrarConfirmacionReservacion(const Reservacion& reservacion, const Alojamiento& alojamiento);
    void mostrarDetalleReservacion(const Reservacion& reserva, int numero);
};

#endif // HUESPED_H
