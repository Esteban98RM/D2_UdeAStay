#ifndef FECHA_H
#define FECHA_H

#include <string>
#include <iostream>

using namespace std;

class Fecha {
private:
    int dia, mes, anio;

public:
    Fecha();
    Fecha(int d, int m, int a);
    Fecha(const string& fechaStr);

    bool esValida() const;
    bool yaPaso() const;

    string toString() const;
    string toStringCompleto() const;
    static string obtenerFechaActual();

    Fecha sumarDias(int diasSumar) const;
    Fecha calcularFechaFinal(int noches) const;

    int aDiasDesdeFechaBase() const;

    // Operadores
    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
    bool operator>=(const Fecha& otra) const;

    friend ostream& operator<<(ostream& os, const Fecha& f);
    friend istream& operator>>(istream& is, Fecha& f);
};

#endif
