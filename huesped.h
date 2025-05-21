#ifndef HUESPED_H
#define HUESPED_H

#include <string>

using namespace std;

class Huesped {
private:

    string documento;
    string clave;
    int antiguedad;
    float puntuacion;

public:

    Huesped();
    Huesped(const string& doc, const string& clv, int ant, float punt);

    // Getters
    const string& getDocumento() const { return documento; }
    const string& getClave() const { return clave; }
    int getAntiguedad() const { return antiguedad; }
    float getPuntuacion() const { return puntuacion; }
};

#endif // HUESPED_H
