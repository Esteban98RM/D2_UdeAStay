#ifndef ANFITRION_H
#define ANFITRION_H

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

    // Getters
    const string& getCodigo() const { return codigo; }
    const string& getDocumento() const { return documento; }
    const string& getClave() const { return clave; }
    int getAntiguedad() const { return antiguedad; }
    float getPuntuacion() const { return puntuacion; }
};

#endif // ANFITRION_H
