#ifndef ANFITRION_H
#define ANFITRION_H

#include "sistema.h"

#include <string>

using namespace std;

class Anfitrion {
private:

    string codigo;
    string documento;
    string clave;
    int antiguedad;
    float puntuacion;

public:

    // Constructor
    Anfitrion();
    Anfitrion(const string& cod, const string& doc, const string& clv, int ant, float punt);

    // Destructor
    ~Anfitrion();

    // Getters
    const string& getCodigo() const { return codigo; }
    const string& getDocumento() const { return documento; }
    const string& getClave() const { return clave; }
    int getAntiguedad() const { return antiguedad; }
    float getPuntuacion() const { return puntuacion; }

    void mostrarReservacionesActivas(Sistema* sistema);
    void cancelarReservacion(Sistema* sistema);
    void actualizarHistorico(Sistema* sistema);

};

#endif // ANFITRION_H
